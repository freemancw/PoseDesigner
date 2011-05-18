/*
===============================================================================
File:       PoseSample.h
Author:     Clinton Freeman
Created:    May 16, 2011
===============================================================================
*/

#ifndef POSESAMPLE_H
#define POSESAMPLE_H

// Qt includes
#include <QtGui>

// system includes
#include <map>
#include <string>

// local includes
#include <KinectInfo.h>
#include <Vector3.h>

// all supposedly horizontal connections between joints are left-to-right.
// all supposedly vertical connections between joints are down-to-up for the
// upper body, and up-to-down for the lower body. it simply was too verbose
// to put this information into the enum names
typedef enum
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
    R_KNEE_FOOT
}
SkeletonVector;

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
