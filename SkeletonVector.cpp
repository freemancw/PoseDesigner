/*
===============================================================================
File:       SkeletonVector.cpp
Author:     Clinton Freeman
Created:    August 17, 2011
===============================================================================
*/

// local
#include <SkeletonVector.h>

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
