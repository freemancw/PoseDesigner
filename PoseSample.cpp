/*
===============================================================================
File:       PoseSample.cpp
Author:     Clinton Freeman
Created:    May 16, 2011
===============================================================================
*/

// Qt
#include <QVector3D>

// OpenNI
#include <XnCppWrapper.h>

// local
#include <PoseSample.h>

static QVector3D vecFromJoints(XnSkeletonJointPosition a, XnSkeletonJointPosition b)
{
    QVector3D vA(a.position.X, a.position.Y, a.position.Z);
    QVector3D vB(b.position.X, b.position.Y, b.position.Z);

    return vA - vB;
}

void PoseSample::calculateVectors()
{
    jVectors[NECK_HEAD] = vecFromJoints(jPositions[XN_SKEL_HEAD],
                                        jPositions[XN_SKEL_NECK]);
    jVectors[NECK_HEAD].normalize();

    jVectors[SHOULDER_SHOULDER] = vecFromJoints(jPositions[XN_SKEL_RIGHT_SHOULDER],
                                                jPositions[XN_SKEL_LEFT_SHOULDER]);
    jVectors[SHOULDER_SHOULDER].normalize();

    jVectors[HIP_HIP] = vecFromJoints(jPositions[XN_SKEL_RIGHT_HIP],
                                      jPositions[XN_SKEL_LEFT_HIP]);
    jVectors[HIP_HIP].normalize();

    jVectors[L_SHOULDER_ELBOW] = vecFromJoints(jPositions[XN_SKEL_LEFT_ELBOW],
                                               jPositions[XN_SKEL_LEFT_SHOULDER]);
    jVectors[L_SHOULDER_ELBOW].normalize();

    jVectors[L_ELBOW_HAND] = vecFromJoints(jPositions[XN_SKEL_LEFT_HAND],
                                           jPositions[XN_SKEL_LEFT_ELBOW]);
    jVectors[L_ELBOW_HAND].normalize();

    jVectors[L_SHOULDER_WAIST] = vecFromJoints(jPositions[XN_SKEL_WAIST],
                                               jPositions[XN_SKEL_LEFT_SHOULDER]);
    jVectors[L_SHOULDER_WAIST].normalize();

    jVectors[L_WAIST_HIP] = vecFromJoints(jPositions[XN_SKEL_LEFT_HIP],
                                          jPositions[XN_SKEL_WAIST]);
    jVectors[L_WAIST_HIP].normalize();

    jVectors[L_HIP_KNEE] = vecFromJoints(jPositions[XN_SKEL_LEFT_KNEE],
                                         jPositions[XN_SKEL_LEFT_HIP]);
    jVectors[L_HIP_KNEE].normalize();

    jVectors[L_KNEE_FOOT] = vecFromJoints(jPositions[XN_SKEL_LEFT_FOOT],
                                          jPositions[XN_SKEL_LEFT_KNEE]);
    jVectors[L_KNEE_FOOT].normalize();

    jVectors[R_SHOULDER_ELBOW] = vecFromJoints(jPositions[XN_SKEL_RIGHT_ELBOW],
                                               jPositions[XN_SKEL_RIGHT_SHOULDER]);
    jVectors[R_SHOULDER_ELBOW].normalize();

    jVectors[R_ELBOW_HAND] = vecFromJoints(jPositions[XN_SKEL_RIGHT_HAND],
                                           jPositions[XN_SKEL_RIGHT_ELBOW]);
    jVectors[R_ELBOW_HAND].normalize();

    jVectors[R_SHOULDER_WAIST] = vecFromJoints(jPositions[XN_SKEL_WAIST],
                                               jPositions[XN_SKEL_RIGHT_SHOULDER]);
    jVectors[R_SHOULDER_WAIST].normalize();

    jVectors[R_WAIST_HIP] = vecFromJoints(jPositions[XN_SKEL_RIGHT_HIP],
                                          jPositions[XN_SKEL_WAIST]);
    jVectors[R_WAIST_HIP].normalize();

    jVectors[R_HIP_KNEE] = vecFromJoints(jPositions[XN_SKEL_RIGHT_KNEE],
                                         jPositions[XN_SKEL_RIGHT_HIP]);
    jVectors[R_HIP_KNEE].normalize();

    jVectors[R_KNEE_FOOT] = vecFromJoints(jPositions[XN_SKEL_RIGHT_FOOT],
                                          jPositions[XN_SKEL_RIGHT_KNEE]);
    jVectors[R_KNEE_FOOT].normalize();
}

/*
QDataStream &operator<<(QDataStream &out, const XnVector3D &p)
{
    qDebug() << "Outputting: XnVector3D";

    out << p.X << p.Y << p.Z;
    return out;
}

QDataStream &operator>>(QDataStream &in, XnVector3D &p)
{
    in >> p.X >> p.Y >> p.Z;
    return in;
}

//XnSkeletonJointPosition
QDataStream &operator<<(QDataStream &out, const XnSkeletonJointPosition &sjp)
{
    qDebug() << "Outputting: XnSkeletonJointPosition";

    out << sjp.position << sjp.fConfidence;
    return out;
}

QDataStream &operator>>(QDataStream &in, XnSkeletonJointPosition &sjp)
{
    in >> sjp.position >> sjp.fConfidence;
    return in;
}

QDataStream &operator<<(QDataStream &out, const PoseSample &p)
{
    qDebug() << "Outputting: PoseSample, " << p.getName();

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

    //qDebug() << name;

    p.setName(name);
    p.setImage(image);
    p.setJPositions(jPositions);
    p.setJVectors(jVectors);

    return in;
}

//XnSkeletonJoint
QDataStream &operator<<(QDataStream &out, XnSkeletonJoint &sj)
{
    qDebug() << "Outputting: XnSkeletonJoint";

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
*/

