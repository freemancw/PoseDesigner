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

kinectInfo_t kinectInfo;

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

    kinectInfo = ki;
}

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

static void __stdcall userLostUser
(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
    qDebug("Lost user %d\n", nId);
}

static void __stdcall userPoseDetected
(xn::PoseDetectionCapability& capability, const XnChar* strPose,
 XnUserID nId, void* pCookie)
{
    qDebug("Pose %s detected for user %d\n", strPose, nId);

    ki.userGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
    ki.userGenerator.GetSkeletonCap().RequestCalibration(nId, true);
}

static void __stdcall userCalibrationStart
(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie)
{
    qDebug("Calibration started for user %d\n", nId);
}

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




/*

// new class stuff

KinectInfo::KinectInfo()
{
    // initialize config values
    bNeedsPose = false;
    bDrawBackground = true;
    bDrawPixels = true;
    bDrawSkeleton = true;
    bPrintID = false;
    bPrintState = false;
    bPaused = false;
    bRecording = false;
    bQuit = false;

    XnStatus nRetVal = XN_STATUS_OK;
    xn::EnumerationErrors errors;

    // attempt to initialize context
    nRetVal = context.InitFromXmlFile(XMLPATH, &errors);
    if(nRetVal == XN_STATUS_NO_NODE_PRESENT)
    {
        XnChar strError[1024];
        errors.ToString(strError, 1024);
        qDebug("%s\n", strError);
        bConnected = false;
        return;
    }
    else if(nRetVal != XN_STATUS_OK)
    {
        qDebug("Open failed: %s\n", xnGetStatusString(nRetVal));
        bConnected = false;
        return;
    }

    // attempt to initialize depth generator
    nRetVal = context.FindExistingNode(XN_NODE_TYPE_DEPTH, depthGenerator);
    CHECK_RC(nRetVal, "Find depth generator");

    // attempt to initialize user generator
    nRetVal = context.FindExistingNode(XN_NODE_TYPE_USER, userGenerator);
    if(nRetVal != XN_STATUS_OK)
    {
        nRetVal = userGenerator.Create(context);
        CHECK_RC(nRetVal, "Find user generator");
    }

    if (!userGenerator.IsCapabilitySupported(XN_CAPABILITY_SKELETON))
    {
        qDebug("Supplied user generator doesn't support skeleton\n");
        return;
    }

    // register callbacks
    XnCallbackHandle hUserCallbacks, hCalibrationCallbacks, hPoseCallbacks;

    userGenerator.RegisterUserCallbacks
    (wrapNewUser, wrapLostUser, NULL, hUserCallbacks);

    userGenerator.GetSkeletonCap().RegisterCalibrationCallbacks
    (wrapCalibrationStart, wrapCalibrationEnd, NULL, hCalibrationCallbacks);

    if(userGenerator.GetSkeletonCap().NeedPoseForCalibration())
    {
        bNeedsPose = true;
        if(userGenerator.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
        {
            qDebug("Pose required, but not supported\n");
            return;
        }

        userGenerator.GetPoseDetectionCap().RegisterToPoseCallbacks
        (wrapPoseDetected, NULL, NULL, hPoseCallbacks);
        userGenerator.GetSkeletonCap().GetCalibrationPose(strPose);
    }

    userGenerator.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);

    nRetVal = context.StartGeneratingAll();
    CHECK_RC(nRetVal, "StartGenerating");
    bConnected = true;
}

KinectInfo::~KinectInfo(){}

// actual callbacks
void KinectInfo::newUser
(xn::UserGenerator &generator, XnUserID nId, void *pCookie)
{
    qDebug("New User %d\n", nId);

    if(bNeedsPose)
        userGenerator.GetPoseDetectionCap().StartPoseDetection(strPose, nId);
    else
        userGenerator.GetSkeletonCap().RequestCalibration(nId, true);
}

void KinectInfo::lostUser
(xn::UserGenerator &generator, XnUserID nId, void *pCookie)
{
    qDebug("Lost user %d\n", nId);
}

void KinectInfo::poseDetected
(xn::PoseDetectionCapability &capability, const XnChar *strPose,
 XnUserID nId, void *pCookie)
{
    qDebug("Pose %s detected for user %d\n", strPose, nId);

    userGenerator.GetPoseDetectionCap().StopPoseDetection(nId);
    userGenerator.GetSkeletonCap().RequestCalibration(nId, true);
}

void KinectInfo::calibrationStart
(xn::SkeletonCapability &capability, XnUserID nId, void *pCookie)
{
    qDebug("Calibration started for user %d\n", nId);
}

void KinectInfo::calibrationEnd
(xn::SkeletonCapability &capability, XnUserID nId,
 XnBool bSuccess, void *pCookie)
{
    if(bSuccess)
    {
        qDebug("Calibration complete, start tracking user %d\n", nId);
        userGenerator.GetSkeletonCap().StartTracking(nId);
    }
    else
    {
        qDebug("Calibration failed for user %d\n", nId);
        if(bNeedsPose)
            userGenerator.GetPoseDetectionCap().StartPoseDetection(strPose, nId);
        else
            userGenerator.GetSkeletonCap().RequestCalibration(nId, true);
    }
}

// singleton stuff
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

// dummy wrappers to pass to OpenNI callbacks, not very interesting
static void __stdcall wrapNewUser
(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
    KinectInfo::getInstance()->newUser(generator, nId, pCookie);
}

static void __stdcall wrapLostUser
(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
{
    KinectInfo::getInstance()->lostUser(generator, nId, pCookie);
}

static void __stdcall wrapPoseDetected
(xn::PoseDetectionCapability& capability, const XnChar* strPose,
 XnUserID nId, void* pCookie)
{
    KinectInfo::getInstance()->poseDetected(capability, strPose, nId, pCookie);
}

static void __stdcall wrapCalibrationStart
(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie)
{
    KinectInfo::getInstance()->calibrationStart(capability, nId, pCookie);
}

static void __stdcall wrapCalibrationEnd
(xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess,
 void* pCookie)
{
    KinectInfo::getInstance()->calibrationEnd(capability, nId, bSuccess, pCookie);
}




*/
















