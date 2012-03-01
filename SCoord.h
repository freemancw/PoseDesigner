/*!
 *  @file SCoord.h
 *  @author Clinton Freeman
 *  @date 3/1/2012
 */

#ifndef SCOORD_H
#define SCOORD_H

#include <QtGui>

namespace PSD
{

struct SCoord
{
    SCoord();
    SCoord(qreal const phi, qreal const theta);
    SCoord operator+(SCoord const& other) const;
    SCoord operator-(SCoord const& other) const;
    SCoord operator*(SCoord const& other) const;
    SCoord operator/(SCoord const& other) const;
    SCoord operator/(int const other) const;
    SCoord& operator+=(SCoord const& other);
    SCoord& operator-=(SCoord const& other);
    SCoord& operator*=(SCoord const& other);
    SCoord& operator/=(SCoord const& other);
    SCoord& operator/=(int const other);

    qreal phi, theta;
};

}

// Debugging
QDebug operator<<(QDebug out, PSD::SCoord const &s);

// Serialization
QDataStream& operator<<(QDataStream &out, PSD::SCoord const &s);
QDataStream& operator>>(QDataStream &in, PSD::SCoord &s);

#endif // SCOORD_H
