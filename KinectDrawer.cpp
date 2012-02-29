/*!
 *  @file KinectDrawer.cpp
 *  @author Clinton Freeman
 *  @date 5/15/2011
 *
 *  Handles displaying Kinect data.
 */

// Qt
#include <QtOpenGL>

// system
#include <math.h>

// local
#include <KinectDrawer.h>
#include <KinectInfo.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

static const int MAXDEPTH = 10000;
static float pDepthHist[MAXDEPTH];

static int nColors = 10;
static float Colors[][3] =
{
    {0.0, 1.0, 1.0},
    {0.0, 0.42, 0.99},
    {0.0, 1.0, 0.0},
    {1.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.0, 0.5, 0.0},
    {0.5, 1.0, 0.0},
    {0.0, 0.5, 1.0},
    {0.5, 0.0, 1.0},
    {1.0, 1.0, 0.5},
    {1.0, 1.0, 1.0}
};

static const float JOINTCONFIDENCE = 0.5;
static GLfloat texcoords[8];

static unsigned int closestPowOfTwo(unsigned int n);

KinectDrawer::KinectDrawer()
{

}

void KinectDrawer::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_MULTISAMPLE);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void KinectDrawer::drawFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    xn::SceneMetaData sceneMD;
    xn::DepthMetaData depthMD;

    if(!kinectInfo.bConnected)
        return;

    kinectInfo.depthGenerator.GetMetaData(depthMD);
    setupProjection(depthMD.XRes(), depthMD.YRes());

    glDisable(GL_TEXTURE_2D);

    if(!kinectInfo.bPause)
    {
        // read next available data
        kinectInfo.context.WaitAndUpdateAll();
    }

    // process new data
    kinectInfo.depthGenerator.GetMetaData(depthMD);
    kinectInfo.userGenerator.GetUserPixels(0, sceneMD);

    setupModelview();
    drawDepthMap(depthMD, sceneMD);

    glFlush();
}

char* pDepthTexBuf;

void KinectDrawer::drawDepthMap
(const xn::DepthMetaData& dmd, const xn::SceneMetaData& smd)
{
    static bool bInitialized = false;
    static GLuint depthTexID;
    //static unsigned char* pDepthTexBuf;
    static int texWidth, texHeight;

    float topLeftX;
    float topLeftY;
    float bottomRightY;
    float bottomRightX;
    float texXpos;
    float texYpos;

    if(!bInitialized)
    {
        texWidth = closestPowOfTwo(dmd.XRes());
        texHeight = closestPowOfTwo(dmd.YRes());

        qDebug("Initializing depth texture: width = %d, height = %d\n",
               texWidth, texHeight);
        depthTexID = initTexture((void**)&pDepthTexBuf, texWidth, texHeight);

        qDebug("Initialized depth texture: width = %d, height = %d\n",
               texWidth, texHeight);
        bInitialized = true;

        topLeftX = dmd.XRes();
        topLeftY = 0;
        bottomRightY = dmd.YRes();
        bottomRightX = 0;
        texXpos = (float)dmd.XRes() / texWidth;
        texYpos = (float)dmd.YRes() / texHeight;

        memset(texcoords, 0, 8 * sizeof(float));
        texcoords[0] = texXpos;
        texcoords[1] = texYpos;
        texcoords[2] = texXpos;
        texcoords[7] = texYpos;
    }

    unsigned int nValue = 0;
    unsigned int nHistValue = 0;
    unsigned int nIndex = 0;
    unsigned int nX = 0;
    unsigned int nY = 0;
    unsigned int nNumberOfPoints = 0;
    XnUInt16 g_nXRes = dmd.XRes();
    XnUInt16 g_nYRes = dmd.YRes();

    char* pDestImage = pDepthTexBuf;

    const XnDepthPixel* pDepth = dmd.Data();
    const XnLabel* pLabels = smd.Data();

    // Calculate the accumulative histogram
    memset(pDepthHist, 0, MAXDEPTH * sizeof(float));
    for(nY = 0; nY < g_nYRes; nY++)
    {
        for(nX = 0; nX < g_nXRes; nX++)
        {
            nValue = *pDepth;
            if(nValue != 0)
            {
                pDepthHist[nValue]++;
                nNumberOfPoints++;
            }

            pDepth++;
        }
    }

    for(nIndex = 1; nIndex < MAXDEPTH; nIndex++)
        pDepthHist[nIndex] += pDepthHist[nIndex-1];

    if(nNumberOfPoints)
    {
        for(nIndex = 1; nIndex < MAXDEPTH; nIndex++)
        {
            pDepthHist[nIndex] = (unsigned int)
            (256 * (1.0 - (pDepthHist[nIndex] / nNumberOfPoints)));
        }
    }

    pDepth = dmd.Data();

    if(kinectInfo.bDrawPixels)
    {
        XnUInt32 nIndex = 0;

        // prepare the texture map
        for(nY = 0; nY < g_nYRes; nY++)
        {
            for(nX = 0; nX < g_nXRes; nX++, nIndex++)
            {
                pDestImage[0] = 0;
                pDestImage[1] = 0;
                pDestImage[2] = 0;

                if(kinectInfo.bDrawBackground || *pLabels != 0)
                {
                    nValue = *pDepth;
                    XnLabel label = *pLabels;
                    XnUInt32 nColorID = label % nColors;
                    if (label == 0)
                    {
                        nColorID = nColors;
                    }

                    if (nValue != 0)
                    {
                        nHistValue = pDepthHist[nValue];

                        pDestImage[0] = nHistValue * Colors[nColorID][0];
                        pDestImage[1] = nHistValue * Colors[nColorID][1];
                        pDestImage[2] = nHistValue * Colors[nColorID][2];
                    }
                }

                pDepth++;
                pLabels++;
                pDestImage += 3;
            }

            pDestImage += (texWidth - g_nXRes) * 3;
        }
    }
    else
    {
        xnOSMemSet(pDepthTexBuf, 0, 3 * 2 * g_nXRes * g_nYRes);
    }

    glBindTexture(GL_TEXTURE_2D, depthTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, pDepthTexBuf);


    // display the OpenGL texture map
    glColor4f(0.75, 0.75, 0.75, 1.0);

    glEnable(GL_TEXTURE_2D);
    this->drawTexture(dmd.XRes(), dmd.YRes(), 0.0, 0.0);
    glDisable(GL_TEXTURE_2D);

    char strLabel[50] = "";
    XnUserID aUsers[15];
    XnUInt16 nUsers = 15;
    kinectInfo.userGenerator.GetUsers(aUsers, nUsers);
    for(int i = 0; i < nUsers; ++i)
    {
        if(kinectInfo.bPrintID)
        {
            XnPoint3D com;
            kinectInfo.userGenerator.GetCoM(aUsers[i], com);
            kinectInfo.depthGenerator.ConvertRealWorldToProjective(1, &com, &com);

            xnOSMemSet(strLabel, 0, sizeof(strLabel));
            if(!kinectInfo.bPrintState)
                sprintf_s(strLabel, "%d", aUsers[i]);
            else if(kinectInfo.userGenerator.GetSkeletonCap().IsTracking(aUsers[i]))
                sprintf_s(strLabel, "%d - Tracking", aUsers[i]);
            else if(kinectInfo.userGenerator.GetSkeletonCap().IsCalibrating(aUsers[i]))
                sprintf_s(strLabel, "%d - Calibrating...", aUsers[i]);
            else
                sprintf_s(strLabel, "%d - Looking for pose", aUsers[i]);

            glColor4f(1.0 - Colors[i%nColors][0], 1.0 - Colors[i%nColors][1],
                      1.0 - Colors[i%nColors][2], 1.0);

            glRasterPos2i(com.X, com.Y);
        }

        if(kinectInfo.bDrawSkeleton &&
           kinectInfo.userGenerator.GetSkeletonCap().IsTracking(aUsers[i]))
        {
            glBegin(GL_LINES);

            glColor4f(1.0 - Colors[aUsers[i]%nColors][0],
                      1.0 - Colors[aUsers[i]%nColors][1],
                      1.0 - Colors[aUsers[i]%nColors][2], 1);

            drawLimb(aUsers[i], XN_SKEL_HEAD, XN_SKEL_NECK);

            drawLimb(aUsers[i], XN_SKEL_NECK, XN_SKEL_LEFT_SHOULDER);
            drawLimb(aUsers[i], XN_SKEL_LEFT_SHOULDER, XN_SKEL_LEFT_ELBOW);
            drawLimb(aUsers[i], XN_SKEL_LEFT_ELBOW, XN_SKEL_LEFT_HAND);

            drawLimb(aUsers[i], XN_SKEL_NECK, XN_SKEL_RIGHT_SHOULDER);
            drawLimb(aUsers[i], XN_SKEL_RIGHT_SHOULDER, XN_SKEL_RIGHT_ELBOW);
            drawLimb(aUsers[i], XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_HAND);

            drawLimb(aUsers[i], XN_SKEL_LEFT_SHOULDER, XN_SKEL_TORSO);
            drawLimb(aUsers[i], XN_SKEL_RIGHT_SHOULDER, XN_SKEL_TORSO);

            drawLimb(aUsers[i], XN_SKEL_TORSO, XN_SKEL_LEFT_HIP);
            drawLimb(aUsers[i], XN_SKEL_LEFT_HIP, XN_SKEL_LEFT_KNEE);
            drawLimb(aUsers[i], XN_SKEL_LEFT_KNEE, XN_SKEL_LEFT_FOOT);

            drawLimb(aUsers[i], XN_SKEL_TORSO, XN_SKEL_RIGHT_HIP);
            drawLimb(aUsers[i], XN_SKEL_RIGHT_HIP, XN_SKEL_RIGHT_KNEE);
            drawLimb(aUsers[i], XN_SKEL_RIGHT_KNEE, XN_SKEL_RIGHT_FOOT);

            drawLimb(aUsers[i], XN_SKEL_LEFT_HIP, XN_SKEL_RIGHT_HIP);

            glEnd();
        }
    }
}

void KinectDrawer::drawTexture
(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
    drawRectangle(topLeftX, topLeftY, bottomRightX, bottomRightY);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void KinectDrawer::drawRectangle
(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
{
    GLfloat verts[8] = { topLeftX, topLeftY,
                         topLeftX, bottomRightY,
                         bottomRightX, bottomRightY,
                         bottomRightX, topLeftY };

    glVertexPointer(2, GL_FLOAT, 0, verts);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glFlush();
}

GLuint KinectDrawer::initTexture(void** buf, int& width, int& height)
{
    GLuint texID = 0;

    glGenTextures(1, &texID);

    width = closestPowOfTwo(width);
    height = closestPowOfTwo(height);

    // rgba = 4
    *buf = new unsigned char[width * height * 4];

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texID;
}

void KinectDrawer::drawLimb
(XnUserID user, XnSkeletonJoint joint1, XnSkeletonJoint joint2)
{
    XnSkeletonJointPosition jointP1, jointP2;

    if(!kinectInfo.userGenerator.GetSkeletonCap().IsTracking(user))
    {
        qDebug("drawLimb: user %d not tracked!\n", user);
        return;
    }

    // get joint positions
    kinectInfo.userGenerator.GetSkeletonCap().GetSkeletonJointPosition
    (user, joint1, jointP1);

    kinectInfo.userGenerator.GetSkeletonCap().GetSkeletonJointPosition
    (user, joint2, jointP2);

    if(jointP1.fConfidence < JOINTCONFIDENCE ||
       jointP2.fConfidence < JOINTCONFIDENCE)
        return;

    XnPoint3D pt[2];
    pt[0] = jointP1.position;
    pt[1] = jointP2.position;
    kinectInfo.depthGenerator.ConvertRealWorldToProjective(2, pt, pt);

    // this function ought to be located within glBegin/glEnd statements
    glVertex3i(pt[0].X, pt[0].Y, 0);
    glVertex3i(pt[1].X, pt[1].Y, 0);
}

void KinectDrawer::setupModelview()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void KinectDrawer::setupProjection(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1.0, 1.0);
}

static unsigned int closestPowOfTwo(unsigned int n)
{
    unsigned int m = 2;
    while(m < n) m <<= 1;

    return m;
}
