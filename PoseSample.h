/*
===============================================================================
File:       PoseSample.h
Author:     Clinton Freeman
Created:    May 16, 2011
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
    inline std::string getName()
    {
        return name;
    }

    inline void setName(std::string& name)
    {
        this->name = name;
    }

    inline QImage getImage()
    {
        return image;
    }

    inline void setImage(QImage& image)
    {
        this->image = image;
    }

    inline std::map<XnSkeletonJoint, XnSkeletonJointPosition>& getJPositions()
    {
        return jPositions;
    }

    inline std::map<SkeletonVector, Vector3>& getJVectors()
    {
        return jVectors;
    }

    inline Vector3& getJVector(SkeletonVector v)
    {
        return jVectors[v];
    }

    void calculateVectors();
};

#endif // POSESAMPLE_H
