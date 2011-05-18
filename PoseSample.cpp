/*
===============================================================================
File:       PoseSample.cpp
Author:     Clinton Freeman
Created:    May 16, 2011
===============================================================================
*/

// OpenNI includes
#include <XnCppWrapper.h>

// local includes
#include <PoseSample.h>
#include <Vector3.h>

void PoseSample::calculateVectors()
{
    jVectors[NECK_HEAD] = Vector3(jPositions[XN_SKEL_HEAD]) -
                          Vector3(jPositions[XN_SKEL_NECK]);

    jVectors[NECK_HEAD].normalize();

    jVectors[SHOULDER_SHOULDER] = Vector3(jPositions[XN_SKEL_RIGHT_SHOULDER]) -
                                  Vector3(jPositions[XN_SKEL_LEFT_SHOULDER]);

    jVectors[SHOULDER_SHOULDER].normalize();

    //qDebug(jVectors[SHOULDER_SHOULDER].toString().c_str());

    jVectors[HIP_HIP] = Vector3(jPositions[XN_SKEL_RIGHT_HIP]) -
                        Vector3(jPositions[XN_SKEL_LEFT_HIP]);

    jVectors[HIP_HIP].normalize();

    //qDebug(jVectors[HIP_HIP].toString().c_str());

    // left

    jVectors[L_SHOULDER_ELBOW] = Vector3(jPositions[XN_SKEL_LEFT_ELBOW]) -
                                 Vector3(jPositions[XN_SKEL_LEFT_SHOULDER]);

    jVectors[L_SHOULDER_ELBOW].normalize();

    jVectors[L_ELBOW_HAND] = Vector3(jPositions[XN_SKEL_LEFT_HAND]) -
                             Vector3(jPositions[XN_SKEL_LEFT_ELBOW]);

    jVectors[L_ELBOW_HAND].normalize();

    jVectors[L_SHOULDER_WAIST] = Vector3(jPositions[XN_SKEL_WAIST]) -
                                 Vector3(jPositions[XN_SKEL_LEFT_SHOULDER]);

    jVectors[L_SHOULDER_WAIST].normalize();

    jVectors[L_WAIST_HIP] = Vector3(jPositions[XN_SKEL_LEFT_HIP]) -
                            Vector3(jPositions[XN_SKEL_WAIST]);

    jVectors[L_WAIST_HIP].normalize();

    jVectors[L_HIP_KNEE] = Vector3(jPositions[XN_SKEL_LEFT_KNEE]) -
                           Vector3(jPositions[XN_SKEL_LEFT_HIP]);

    jVectors[L_HIP_KNEE].normalize();

    jVectors[L_KNEE_FOOT] = Vector3(jPositions[XN_SKEL_LEFT_FOOT]) -
                            Vector3(jPositions[XN_SKEL_LEFT_KNEE]);

    jVectors[L_KNEE_FOOT].normalize();

    // right

    jVectors[R_SHOULDER_ELBOW] = Vector3(jPositions[XN_SKEL_RIGHT_ELBOW]) -
                                 Vector3(jPositions[XN_SKEL_RIGHT_SHOULDER]);

    jVectors[R_SHOULDER_ELBOW].normalize();

    jVectors[R_ELBOW_HAND] = Vector3(jPositions[XN_SKEL_RIGHT_HAND]) -
                             Vector3(jPositions[XN_SKEL_RIGHT_ELBOW]);

    jVectors[R_ELBOW_HAND].normalize();

    jVectors[R_SHOULDER_WAIST] = Vector3(jPositions[XN_SKEL_WAIST]) -
                                 Vector3(jPositions[XN_SKEL_RIGHT_SHOULDER]);

    jVectors[R_SHOULDER_WAIST].normalize();

    jVectors[R_WAIST_HIP] = Vector3(jPositions[XN_SKEL_RIGHT_HIP]) -
                            Vector3(jPositions[XN_SKEL_WAIST]);

    jVectors[R_WAIST_HIP].normalize();

    jVectors[R_HIP_KNEE] = Vector3(jPositions[XN_SKEL_RIGHT_KNEE]) -
                           Vector3(jPositions[XN_SKEL_RIGHT_HIP]);

    jVectors[R_HIP_KNEE].normalize();

    jVectors[R_KNEE_FOOT] = Vector3(jPositions[XN_SKEL_RIGHT_FOOT]) -
                            Vector3(jPositions[XN_SKEL_RIGHT_KNEE]);

    jVectors[R_KNEE_FOOT].normalize();
}
