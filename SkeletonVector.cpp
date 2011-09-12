/*!
 *  @file SkeletonVector.cpp
 *  @author Clinton Freeman
 *  @date 8/20/2011
 *
 *  SkeletonVector implementation. See SkeletonVector.h for a detailed
 *  description.
 */

// Qt
#include <QtCore>

// local
#include <SkeletonVector.h>

static const QString SkeletonVectorNames[] =
{
    "NECK_HEAD",
    "SHOULDER_SHOULDER",
    "HIP_HIP",
    "L_SHOULDER_ELBOW",
    "L_ELBOW_HAND",
    "L_SHOULDER_WAIST",
    "L_WAIST_HIP",
    "L_HIP_KNEE",
    "L_KNEE_FOOT",
    "R_SHOULDER_ELBOW",
    "R_ELBOW_HAND",
    "R_SHOULDER_WAIST",
    "R_WAIST_HIP",
    "R_HIP_KNEE",
    "R_KNEE_FOOT",
    "SKEL_VEC_MAX"
};

QDebug operator<<(QDebug d, SkeletonVector &sv)
{
    d << SkeletonVectorNames[sv];
    return d;
}

QString &operator<<(QString &s, SkeletonVector &sv)
{
    s = SkeletonVectorNames[sv];
    return s;
}

QDataStream &operator<<(QDataStream &out, const SkeletonVector &sv)
{
    out << quint32(sv);
    return out;
}

QDataStream &operator>>(QDataStream &in, SkeletonVector &sv)
{
    quint32 sin;
    in >> sin;
    sv = (SkeletonVector)sin;
    return in;
}
