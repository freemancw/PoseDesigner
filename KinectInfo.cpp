/*
===============================================================================
File:       KinectInfo.cpp
Author:     Clinton Freeman
Created:    May 14, 2011
===============================================================================
*/

// Qt
#include <QDebug>

// local
#include <KinectInfo.h>

kinectInfo_t ki;

// callbacks
static void __stdcall userNewUser
(xn::UserGenerator& generator, XnUserID nId, void* pCookie);

static void __stdcall userLostUser
(xn::UserGenerator& generator, XnUserID nId, void* pCookie);

static void __stdcall userPoseDetected
(xn::PoseDetectionCapability& capability, const XnChar* strPose,
 XnUserID nId, void* pCookie);

static void __stdcall userCalibrationStart
(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie);

static void __stdcall userCalibrationEnd
(xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess,
 void* pCookie);

#define CHECK_RC(nRetVal, what)						     \
        if (nRetVal != XN_STATUS_OK)      				     \
        {								     \
            qDebug("%s failed: %s\n", what, xnGetStatusString(nRetVal));     \
            return;                                                          \
        }

static const char * XMLPATH =
"C:/Program Files (x86)/OpenNI/Data/SamplesConfig.xml";

/**
 * Connect with device, register callbacks.
 */
void initKinect()
{
    // initialize config values
    ki.bNeedPose = false;
    ki.bDrawBackground = true;
    ki.bDrawPixels = true;
    ki.bDrawSkeleton = true;
    ki.bPrintID = false;
    ki.bPrintState = false;
    ki.bPause = false;
    ki.bRecord = false;
    ki.bQuit = false;

    XnStatus nRetVal = XN_STATUS_OK;
    xn::EnumerationErrors errors;

    // attempt to initialize context
    nRetVal = ki.context.InitFromXmlFile(XMLPATH, &errors);
    if(nRetVal == XN_STATUS_NO_NODE_PRESENT)
    {
        XnChar strError[1024];
        errors.ToString(strError, 1024);
        qDebug("%s\n", strError);
        ki.bConnected = false;
        return;
    }
    else if(nRetVal != XN_STATUS_OK)
    {
        qDebug("Open failed: %s\n", xnGetStatusString(nRetVal));
        ki.bConnected = false;
        return;
    }

    // attempt to initialize depth generator
    nRetVal = ki.context.FindExistingNode(XN_NODE_TYPE_DEPTH, ki.depthGenerator);
    CHECK_RC(nRetVal, "Find depth generator");

    // attempt to initialize user generator
    nRetVal = ki.context.FindExistingNode(XN_NODE_TYPE_USER, ki.userGenerator);
    if(nRetVal != XN_STATUS_OK)
    {
        nRetVal = ki.userGenerator.Create(ki.context);
        CHECK_RC(nRetVal, "Find user generator");
    }

    if (!ki.userGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
    {
        qDebug("Supplied user generator doesn't support skeleton\n");
        return;
    }

    // register callbacks
    XnCallbackHandle hUserCallbacks, hCalibrationCallbacks, hPoseCallbacks;

    ki.userGenerator.RegisterUserCallbacks
    (userNewUser, userLostUser, NULL, hUserCallbacks);

    ki.userGenerator.GetSkeletonCap().RegisterCalibrationCallbacks
    (userCalibrationStart, userCalibrationEnd, NULL, hCalibrationCallbacks);

    if(ki.userGenerator.GetSkeletonCap().NeedPoseForCalibration())
    {
        ki.bNeedPose = true;
        if(!ki.userGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
        {
            qDebug("Pose required, but not supported\n");
            return;
        }

        ki.userGenerator.GetPoseDetectionCap().RegisterToPoseCallbacks
        (userPoseDetected, NULL, NULL, hPoseCallbacks);

        ki.userGenerator.GetSkeletonCap().GetCalibrationPose(ki.strPose);
    }

    ki.userGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

    nRetVal = ki.context.StartGeneratingAll();
    CHECK_RC(nRetVal, "StartGenerating");
    ki.bConnected = true;
}

/**
 * A new user is found.
 */
static void __stdcall userNewUser
(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
    qDebug("New User %d\n", nId);

    // new user found
    if(ki.bNeedPose)
        ki.userGenerator.GetPoseDetectionCap().StartPoseDetection(ki.strPose, nId);
    else
        ki.userGenerator.GetSkeletonCap().RequestCalibration(nId, true);
}

/**
 * A user is lost.
 */
static void __stdcall userLostUser
(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
    qDebug("Lost user %d\n", nId);
}

/**
 * A pose was detected for a certain user.
 */
static void __stdcall userPoseDetected
(xn::PoseDetectionCapability& capability, const XnChar* strPose,
 XnUserID nId, void* pCookie)
{
    qDebug("Pose %s detected for user %d\n", strPose, nId);

    ki.userGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
    ki.userGenerator.GetSkeletonCap().RequestCalibration(nId, true);
}

/**
 * Calibration has started for a certain user.
 */
static void __stdcall userCalibrationStart
(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie)
{
    qDebug("Calibration started for user %d\n", nId);
}

/**
 * Calibration either succeeded or failed for a certain user.
 */
static void __stdcall userCalibrationEnd
(xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess,
 void* pCookie)
{
    if(bSuccess)
    {
        // calibration succeeded
        qDebug("Calibration complete, start tracking user %d\n", nId);
        ki.userGenerator.GetSkeletonCap().StartTracking(nId);
    }
    else
    {
        // calibration failed
        qDebug("Calibration failed for user %d\n", nId);
        if(ki.bNeedPose)
            ki.userGenerator.GetPoseDetectionCap().StartPoseDetection(ki.strPose, nId);
        else
            ki.userGenerator.GetSkeletonCap().RequestCalibration(nId, true);
    }
}






// new class stuff

KinectInfo::KinectInfo()
{
    // do stuff
}

KinectInfo::~KinectInfo()
{
    // undo stuff
}

KinectInfo* KinectInfo::instance = NULL;

KinectInfo* KinectInfo::getInstance()
{
    if(instance == NULL)
        instance = new KinectInfo();

    return instance;
}

void KinectInfo::destroyInstance()
{
    delete instance;
}





















