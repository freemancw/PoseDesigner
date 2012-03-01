/*!
 *  @file PoseSample.h
 *  @author Clinton Freeman
 *  @date 5/16/2011
 */

#ifndef POSESAMPLE_H
#define POSESAMPLE_H

// Qt
#include <QtGui>
#include <QVector3D>

// local
#include <KinectInfo.h>
#include <SCoord.h>
#include <SkeletonVector.h>

namespace PSD
{

struct TorsoFrame
{
    QVector3D u, r, t;
};

class PoseSample
{
public:
    PoseSample();
    void calcCoords();
    QString const& getName() const;
    QImage const& getImage() const;
    QMap<XnSkeletonJoint, XnSkeletonJointPosition> const& getJPositions() const;
    QMap<XnSkeletonJoint, XnSkeletonJointPosition>& getJPositions_nc(); // OpenNI is dumb
    XnSkeletonJointPosition const getJPosition(XnSkeletonJoint const sj) const;
    QMap<SkeletonVector, SCoord> const& getJCoords() const;
    SCoord const getJCoord(SkeletonVector const sv) const;
    void setName(QString const &name);
    void setImage(QImage const &image);
    void setJPositions(QMap<XnSkeletonJoint, XnSkeletonJointPosition> const &jPositions);
    void setJPosition(XnSkeletonJoint const sj, XnSkeletonJointPosition const &jp);
    void setJCoords(QMap<SkeletonVector, SCoord> const &jCoords);
    void setJCoord(SkeletonVector const sv, SCoord const &c);
    void setJCoordPhi(SkeletonVector const sv, qreal const phi);
    void setJCoordTheta(SkeletonVector const sv, qreal const theta);

private:
    QString name;
    QImage image;
    TorsoFrame torsoFrame;
    QMap<XnSkeletonJoint, XnSkeletonJointPosition> jPositions;
    QMap<SkeletonVector, SCoord> jCoords;
};

}

// Debugging
QDebug operator<<(QDebug d, XnSkeletonJoint const &sj);
QDebug operator<<(QDebug d, XnSkeletonJointPosition const &jp);
QDebug operator<<(QDebug d, XnVector3D const &v);
QDebug operator<<(QDebug d, PSD::PoseSample const &ps);

// Serialization
QDataStream& operator<<(QDataStream &out, XnSkeletonJoint const &sj);
QDataStream& operator>>(QDataStream &in, XnSkeletonJoint &sj);
QDataStream& operator<<(QDataStream &out, XnSkeletonJointPosition const &jp);
QDataStream& operator>>(QDataStream &in, XnSkeletonJointPosition &jp);
QDataStream& operator<<(QDataStream &out, XnVector3D const &v);
QDataStream& operator>>(QDataStream &in, XnVector3D &v);
QDataStream& operator<<(QDataStream &out, PSD::PoseSample const &ps);
QDataStream& operator>>(QDataStream &in, PSD::PoseSample &ps);

// Misc
inline XnSkeletonJoint& operator++(XnSkeletonJoint& sj, int)
{
    int temp = sj;
    return sj = static_cast<XnSkeletonJoint>(++temp);
}

#endif // POSESAMPLE_H
