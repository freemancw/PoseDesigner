/*
===============================================================================
File:       PoseSample.cpp
Author:     Clinton Freeman
Created:    May 16, 2011
===============================================================================
*/

// OpenNI
#include <XnCppWrapper.h>

// local
#include <PoseSample.h>
#include <Vector3.h>

QDataStream &operator<<(QDataStream &out, const XnVector3D &p)
{
    out << p.X << p.Y << p.Z;
    return out;
}

QDataStream &operator>>(QDataStream &in, XnVector3D &p)
{
    in >> p.X >> p.Y >> p.Z;
    return in;
}


void PoseSample::calculateVectors()
{
    jVectors[NECK_HEAD] = Vector3(jPositions[XN_SKEL_HEAD]) -
                          Vector3(jPositions[XN_SKEL_NECK]);

    jVectors[NECK_HEAD].normalize();

    jVectors[SHOULDER_SHOULDER] = Vector3(jPositions[XN_SKEL_RIGHT_SHOULDER]) -
                                  Vector3(jPositions[XN_SKEL_LEFT_SHOULDER]);

    jVectors[SHOULDER_SHOULDER].normalize();

    jVectors[HIP_HIP] = Vector3(jPositions[XN_SKEL_RIGHT_HIP]) -
                        Vector3(jPositions[XN_SKEL_LEFT_HIP]);

    jVectors[HIP_HIP].normalize();

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


//XnSkeletonJointPosition
QDataStream &operator<<(QDataStream &out, const XnSkeletonJointPosition &sjp)
{
    out << sjp.position << sjp.fConfidence;
    return out;
}

QDataStream &operator>>(QDataStream &in, XnSkeletonJointPosition &sjp)
{
    in >> sjp.position >> sjp.fConfidence;
    return in;
}

QDataStream &operator<<(QDataStream &out, PoseSample &p)
{
    out << p.getName() << p.getImage() << p.getJPositions() << p.getJVectors();
    return out;
}

QDataStream &operator>>(QDataStream &in, PoseSample &p)
{
    QString name;
    QImage image;
    QMap<XnSkeletonJoint, XnSkeletonJointPosition> jPositions;
    QMap<SkeletonVector, Vector3> jVectors;

    in >> name >> image >> jPositions >> jVectors;

    p.setName(name);
    p.setImage(image);
    p.setJPositions(jPositions);
    p.setJVectors(jVectors);

    return in;
}

//XnSkeletonJoint
QDataStream &operator<<(QDataStream &out, XnSkeletonJoint &sj)
{
    out << quint32(sj);
    return out;
}

QDataStream &operator>>(QDataStream &in, XnSkeletonJoint &sj)
{
    quint32 sin;
    in >> sin;
    sj = (XnSkeletonJoint)sin;
    return in;
}

