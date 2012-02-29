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

/*
===============================================================================
Debugging / Unit Tests
===============================================================================
*/

static QString const SkeletonVectorNames[] =
{
    "NECK_HEAD",
    "L_SHOULDER_ELBOW",
    "L_ELBOW_HAND",
    "L_HIP_KNEE",
    "L_KNEE_FOOT",
    "R_SHOULDER_ELBOW",
    "R_ELBOW_HAND",
    "R_HIP_KNEE",
    "R_KNEE_FOOT",
    "SKEL_VEC_MAX"
};

QDebug operator<<(QDebug d, SkeletonVector const &sv)
{
    d << SkeletonVectorNames[sv];
    return d;
}

QString &operator<<(QString &s, SkeletonVector const &sv)
{
    s = SkeletonVectorNames[sv];
    return s;
}

/*
===============================================================================
Object Serialization
===============================================================================
*/

QDataStream &operator<<(QDataStream &out, SkeletonVector const &sv)
{
    out << quint32(sv);
    return out;
}

QDataStream &operator>>(QDataStream &in, SkeletonVector &sv)
{
    quint32 sin;
    in >> sin;
    sv = static_cast<SkeletonVector>(sin);
    return in;
}
