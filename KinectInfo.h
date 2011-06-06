/*
===============================================================================
File:       KinectInfo.h
Author:     Clinton Freeman
Created:    May 14, 2011
===============================================================================
*/

#ifndef KINECTINFO_H
#define KINECTINFO_H

// OpenNI
#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>

class KinectInfo
{
private:
    static KinectInfo* instance;

    xn::Context context;
    xn::DepthGenerator depthGenerator;
    xn::UserGenerator userGenerator;

    XnChar strPose[20];

    XnBool bNeedsPose;
    XnBool bDrawBackground;
    XnBool bDrawPixels;
    XnBool bDrawSkeleton;
    XnBool bPrintID;
    XnBool bPrintState;
    XnBool bPaused;
    XnBool bRecording;
    XnBool bQuit;
    XnBool bConnected;

    // actual callbacks
    void userNewUser
    (xn::UserGenerator& generator, XnUserID nId, void* pCookie);
    void userLostUser
    (xn::UserGenerator& generator, XnUserID nId, void* pCookie);
    void userPoseDetected
    (xn::PoseDetectionCapability& capability, const XnChar* strPose,
     XnUserID nId, void* pCookie);
    void userCalibrationStart
    (xn::SkeletonCapability& capability, XnUserID nId, void* pCookie);
    void userCalibrationEnd
    (xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess,
     void* pCookie);

    // wrappers
    friend void __stdcall userNewUser
    (xn::UserGenerator& generator, XnUserID nId, void* pCookie);
    friend void __stdcall userLostUser
    (xn::UserGenerator& generator, XnUserID nId, void* pCookie);
    friend void __stdcall userPoseDetected
    (xn::PoseDetectionCapability& capability, const XnChar* strPose,
     XnUserID nId, void* pCookie);
    friend void __stdcall userCalibrationStart
    (xn::SkeletonCapability& capability, XnUserID nId, void* pCookie);
    friend void __stdcall userCalibrationEnd
    (xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess,
     void* pCookie);

public:
    // getters
    inline xn::Context& getContext() { return context; }
    inline xn::DepthGenerator& getDepthGenerator() { return depthGenerator; }
    inline xn::UserGenerator& getUserGenerator() { return userGenerator; }

    inline XnBool needsPose() { return bNeedsPose; }
    inline XnBool shouldDrawBackground() { return bDrawBackground; }
    inline XnBool shouldDrawPixels() { return bDrawPixels; }
    inline XnBool shouldDrawSkeleton() { return bDrawSkeleton; }
    inline XnBool shouldPrintID() { return bPrintID; }
    inline XnBool shouldPrintState() { return bPrintState; }
    inline XnBool isPaused() { return bPaused; }
    inline XnBool isRecording() { return bRecording; }
    inline XnBool shouldQuit() { return bQuit; }
    inline XnBool isConnected() { return bConnected; }

    // setters
    inline void setNeedsPose(XnBool b) { bNeedsPose = b; }
    inline void setDrawBackground(XnBool b) { bDrawBackground = b; }
    inline void setDrawPixels(XnBool b) { bDrawPixels = b; }
    inline void setDrawSkeleton(XnBool b) { bDrawSkeleton = b; }
    inline void setPrintID(XnBool b) { bPrintID = b; }
    inline void setPrintState(XnBool b) { bPrintState = b; }
    inline void setPaused(XnBool b) { bPaused = b; }
    inline void setRecording(XnBool b) { bRecording = b; }
    inline void setQuit(XnBool b) { bQuit = b; }
    inline void setConnected(XnBool b) { bConnected = b; }

    static KinectInfo* getInstance();
    static void destroyInstance();

protected:
    KinectInfo();
    ~KinectInfo();
};


struct kinectInfo_t
{
    xn::Context context;
    xn::DepthGenerator depthGenerator;
    xn::UserGenerator userGenerator;

    XnChar strPose[20];

    XnBool bNeedPose;
    XnBool bDrawBackground;
    XnBool bDrawPixels;
    XnBool bDrawSkeleton;
    XnBool bPrintID;
    XnBool bPrintState;
    XnBool bPause;
    XnBool bRecord;
    XnBool bQuit;
    XnBool bConnected;
};

extern kinectInfo_t ki;

void initKinect();

#endif // KINECTINFO_H
