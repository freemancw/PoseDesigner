/*
===============================================================================
File:       SkeletonVector.h
Author:     Clinton Freeman
Created:    May 20, 2011

Comments:   A SkeletonVector is just a useful enumeration for identifying the
            3D vectors generated inside of PoseSamples. A convention for this
            was not provided by OpenNI, so I just made one up. The names are
            fairly obvious, an example being L_SHOULDER_ELBOW, which identifies
            a vector that originates at the left shoulder and extends to the
            left elbow. As is typically the case with 3D conventions it
            shouldn't matter exactly what goes where so long as you're
            consistent.
===============================================================================
*/

#ifndef SKELETONVECTOR_H
#define SKELETONVECTOR_H

// Qt
#include <QDataStream>

enum SkeletonVector
{
    NECK_HEAD,
    SHOULDER_SHOULDER,
    HIP_HIP,
    L_SHOULDER_ELBOW,
    L_ELBOW_HAND,
    L_SHOULDER_WAIST,
    L_WAIST_HIP,
    L_HIP_KNEE,
    L_KNEE_FOOT,
    R_SHOULDER_ELBOW,
    R_ELBOW_HAND,
    R_SHOULDER_WAIST,
    R_WAIST_HIP,
    R_HIP_KNEE,
    R_KNEE_FOOT,
    SKEL_VEC_MAX
};

QDataStream &operator<<(QDataStream &, const SkeletonVector &);
QDataStream &operator>>(QDataStream &, SkeletonVector &);

//extern const char **SkeletonVectorNames;

// perhaps change this at some point to look the same as the OpenNI stuff,
// since we cant change those enums... i.e. don't use a MAX value to stop,
// and have the for loop termination condition be <= instead of just <
inline SkeletonVector& operator++(SkeletonVector& sv, int)
{
    if(sv == SKEL_VEC_MAX)
        return sv = NECK_HEAD;

    int temp = sv;
    return sv = static_cast<SkeletonVector>(++temp);
}

#endif // SKELETONVECTOR_H
