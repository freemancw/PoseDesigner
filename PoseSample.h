/*
===============================================================================
File:       PoseSample.h
Author:     Clinton Freeman
Created:    May 16, 2011

Comments:   A PoseSample is composed of several things, the most important of
            which is a collection of 3D vectors that go from a particular joint
            to another.

            Operating under the assumption that the player has been recognized
            and his skeleton is being tracked, the way a sample is taken is
            that each of the joint positions are retrieved through OpenNI at
            the time of sampling and stored in a std::map. These positions are
            identified by the XnSkeletonJoint enumeration provided by OpenNI.

            From the position data, the 3D vectors are then generated according
            to a somewhat arbitrary convention (AFAICT it doesn't actually
            matter how this is done). This convention is codified inside of the
            SkeletonVector enumeration and elaborated inside comments in that
            file. The vectors are stored in a std::map, identified by the
            particular SkeletonVector that they represent.

            Additionally - primarily because Qt makes it so damned easy - an
            image is "snapped" from the OpenGL framebuffer at sampling time
            and is used so that the user can actually remember the pose they
            are working on; this is especially useful for when you come back to
            a pose after some time. Qt can also easily export this image to
            a common image format, such as JPG.
===============================================================================
*/

#ifndef POSESAMPLE_H
#define POSESAMPLE_H

// Qt
#include <QtGui>

// system
#include <map>
#include <string>

// local
#include <KinectInfo.h>
#include <SkeletonVector.h>
#include <Vector3.h>

inline XnSkeletonJoint& operator++(XnSkeletonJoint& sj, int)
{
    int temp = sj;
    return sj = static_cast<XnSkeletonJoint>(++temp);
}

class PoseSample
{
private:
    std::string name;
    QImage image;

    std::map<XnSkeletonJoint, XnSkeletonJointPosition> jPositions;
    std::map<SkeletonVector, Vector3> jVectors;

public:
    void calculateVectors();

    // getters
    inline std::string getName() { return name; }
    inline QImage getImage() { return image; }
    inline std::map<XnSkeletonJoint, XnSkeletonJointPosition>& getJPositions()
    { return jPositions; }
    inline std::map<SkeletonVector, Vector3>& getJVectors()
    { return jVectors; }
    inline Vector3& getJVector(SkeletonVector v) { return jVectors[v]; }

    // setters
    inline void setName(std::string& name) { this->name = name; }
    inline void setImage(QImage& image) { this->image = image; }
};

#endif // POSESAMPLE_H
