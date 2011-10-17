/*!
 *  @file PoseSample.h
 *  @author Clinton Freeman
 *  @date 5/16/2011
 *
 *  A PoseSample is composed of several things, the most important of which is
 *  a collection of 3D vectors that go from a particular joint to another.
 *
 *  Operating under the assumption that the player has been recognized and his
 *  skeleton is being tracked, the way a sample is taken is that each of the
 *  joint positions are retrieved through OpenNI at the time of sampling and
 *  stored in a std::map. These positions are identified by the XnSkeletonJoint
 *  enumeration provided by OpenNI.
 *
 *  From the position data, the 3D vectors are then generated according to a
 *  somewhat arbitrary convention (AFAICT it doesn't actually matter how this
 *  is done). This convention is codified inside of the SkeletonVector
 *  enumeration and elaborated inside comments in SkeletonVector.h. The vectors
 *  are stored in a std::map, identified by the particular SkeletonVector that
 *  they represent.

 *  Additionally - primarily because Qt makes it so damned easy - an image is
 *  "snapped" from the OpenGL framebuffer at sampling time and is used so that
 *  the user can actually remember the pose they are working on; this is
 *  especially useful for when you come back to a pose after some time. Qt can
 *  also easily export this image to a common image format, such as JPG or PNG.
 */

#ifndef POSESAMPLE_H
#define POSESAMPLE_H

// Qt
#include <QtGui>
#include <QVector3D>

// local
#include <KinectInfo.h>
#include <SkeletonVector.h>

struct SphericalCoords
{
    float theta, phi;
};

struct TorsoFrame
{
    QVector3D u, r, t;
};

class PoseSample
{
private:
    QString name;
    QImage image;

    TorsoFrame torsoFrame;

    QMap<XnSkeletonJoint, XnSkeletonJointPosition> jPositions;
    QMap<SkeletonVector, SphericalCoords> jCoords;
    QMap<SkeletonVector, QVector3D> jVectors; //! @todo remove this

public:
    void calculateVectors();
    void calculateCoords();

    // getters
    inline QString getName() const
    { return name; }

    inline QImage getImage() const
    { return image; }

    inline QMap<XnSkeletonJoint, XnSkeletonJointPosition> const & getJPositions() const
    { return jPositions; }

    inline QMap<XnSkeletonJoint, XnSkeletonJointPosition>& getJPositions_nc()
    { return jPositions; }

    inline QMap<SkeletonVector, QVector3D> const & getJVectors() const
    { return jVectors; }

    inline QVector3D& getJVector(SkeletonVector sv)
    { return jVectors[sv]; }

    // setters
    inline void setName(QString const &name) { this->name = name; }
    inline void setImage(QImage const &image) { this->image = image; }
    inline void setJPositions(QMap<XnSkeletonJoint, XnSkeletonJointPosition> const &jPositions)
    { this->jPositions = jPositions; }
    inline void setJVectors(QMap<SkeletonVector, QVector3D> const &jVectors)
    { this->jVectors = jVectors; }
    inline void setJVector(SkeletonVector sv, QVector3D const &v) { jVectors[sv] = v; }
};

inline XnSkeletonJoint& operator++(XnSkeletonJoint& sj, int)
{
    int temp = sj;
    return sj = static_cast<XnSkeletonJoint>(++temp);
}

QDebug operator<<(QDebug, XnSkeletonJoint &);
QDataStream &operator<<(QDataStream &, XnSkeletonJoint &);
QDataStream &operator>>(QDataStream &, XnSkeletonJoint &);

QDebug operator<<(QDebug, XnSkeletonJointPosition &);
QDataStream &operator<<(QDataStream &, const XnSkeletonJointPosition &);
QDataStream &operator>>(QDataStream &, XnSkeletonJointPosition &);

QDebug operator<<(QDebug, XnVector3D &);
QDataStream &operator<<(QDataStream &, const XnVector3D &);
QDataStream &operator>>(QDataStream &, XnVector3D &);

QDebug operator<<(QDebug, PoseSample &);
QDataStream &operator<<(QDataStream &, const PoseSample &);
QDataStream &operator>>(QDataStream &, PoseSample &);


#endif // POSESAMPLE_H
