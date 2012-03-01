/*!
 *  @file SCoord.cpp
 *  @author Clinton Freeman
 *  @date 3/1/2012
 */

#include <SCoord.h>

using PSD::SCoord;

/*
===============================================================================
Constructors/Destructors
===============================================================================
*/

SCoord::SCoord() :
    phi(0.0), theta(0.0) {}

SCoord::SCoord(qreal const phi, qreal const theta) :
    phi(phi), theta(theta) {}

/*
===============================================================================
Operators
===============================================================================
*/

SCoord SCoord::operator+(SCoord const& other) const
{
    return SCoord(phi + other.phi, theta + other.theta);
}

SCoord SCoord::operator-(SCoord const& other) const
{
    return SCoord(phi - other.phi, theta - other.theta);
}

SCoord SCoord::operator*(SCoord const& other) const
{
    return SCoord(phi * other.phi, theta * other.theta);
}

SCoord SCoord::operator/(SCoord const& other) const
{
    return SCoord(phi / other.phi, theta / other.theta);
}

SCoord SCoord::operator/(int const other) const
{
    return SCoord(phi / static_cast<qreal>(other),
                  theta / static_cast<qreal>(other));
}

SCoord& SCoord::operator+=(SCoord const& other)
{
    phi += other.phi;
    theta += other.theta;
    return *this;
}

SCoord& SCoord::operator-=(SCoord const& other)
{
    phi -= other.phi;
    theta -= other.theta;
    return *this;
}

SCoord& SCoord::operator*=(SCoord const& other)
{
    phi *= other.phi;
    theta *= other.theta;
    return *this;
}

SCoord& SCoord::operator/=(SCoord const& other)
{
    phi /= other.phi;
    theta /= other.theta;
    return *this;
}

SCoord& SCoord::operator/=(int const other)
{
    phi /= static_cast<qreal>(other);
    theta /= static_cast<qreal>(other);
    return *this;
}

/*
===============================================================================
Object Serialization
===============================================================================
*/

QDataStream& operator<<(QDataStream &out, SCoord const &s)
{
    out << s.phi << s.theta;
    return out;
}

QDataStream& operator>>(QDataStream &in, SCoord &s)
{
    in >> s.phi >> s.theta;
    return in;
}

/*
===============================================================================
Debugging
===============================================================================
*/

//! @todo magic characters (0x03A6 = unicode phi, 0x03B8 = unicode theta)
QDebug operator<<(QDebug out, SCoord const &s)
{
    out << QChar(0x03A6) << ": " << s.phi << ", "
        << QChar(0x03B8) << ": " << s.theta;
    return out;
}
