/*
===============================================================================
File:		Vector3.cpp
Author: 	Clinton Freeman
Created: 	May 7, 2011
===============================================================================
*/

// local
#include <Vector3.h>

const Vector3 Vector3::XAXIS = Vector3(1.0, 0.0, 0.0);
const Vector3 Vector3::YAXIS = Vector3(0.0, 1.0, 0.0);
const Vector3 Vector3::ZAXIS = Vector3(0.0, 0.0, 1.0);
const Vector3 Vector3::ZERO  = Vector3(0.0, 0.0, 0.0);

QDataStream &operator<<(QDataStream &out, const Vector3 &v)
{
    out << v.x << v.y << v.z;
    return out;
}

QDataStream &operator>>(QDataStream &in, Vector3 &v)
{
    float x, y, z;
    in >> x >> y >> z;
    v = Vector3(x, y, z);
    return in;
}
