/*
===============================================================================
File:       KinectDrawer.h
Author:     Clinton Freeman
Created:    May 15, 2011

Comments:
===============================================================================
*/

#ifndef KINECTDRAWER_H
#define KINECTDRAWER_H

// Qt
#include <QtOpenGL>

// OpenNI
#include <XnCppWrapper.h>

extern char* pDepthTexBuf;

class KinectDrawer
{
    GLuint initTexture(void** buf, int& width, int& height);

    void drawRectangle
    (float topLeftX, float topLeftY, float bottomRightX, float bottomRightY);

    void drawTexture
    (float topLeftX, float topLeftY, float bottomRightX, float bottomRightY);

    void drawLimb
    (XnUserID user, XnSkeletonJoint joint1, XnSkeletonJoint joint2);

    void drawSkeleton(XnUserID user);

    void drawDepthMap
    (const xn::DepthMetaData& dmd, const xn::SceneMetaData& smd);

    void setupProjection(int width, int height);
    void setupModelview();

public:
    KinectDrawer();

    void initializeGL();
    void drawFrame();
};

#endif // KINECTDRAWER_H
