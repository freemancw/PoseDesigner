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
