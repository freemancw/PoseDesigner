/*
===============================================================================
File:       Pose.cpp
Author:     Clinton Freeman
Created:    May 19, 2011
===============================================================================
*/

// system
#include <string>
#include <map>
#include <math.h>

// local
#include <Pose.h>
#include <PoseSample.h>
#include <Vector3.h>

void Pose::calculateStatistics()
{
    for(SkeletonVector sv = NECK_HEAD; sv < SKEL_VEC_MAX; sv++)
    {
        mean.getJVector(sv) = stddev.getJVector(sv) = Vector3::ZERO;
        std::map<std::string, PoseSample>::iterator iter;

        for(iter = samples.begin(); iter != samples.end(); iter++)
            mean.getJVector(sv) += iter->second.getJVector(sv);

        mean.getJVector(sv) /= samples.size();

        for(iter = samples.begin(); iter != samples.end(); iter++)
        {
            stddev.getJVector(sv) += (iter->second.getJVector(sv) - mean.getJVector(sv)) *
                                     (iter->second.getJVector(sv) - mean.getJVector(sv));
        }

        stddev.getJVector(sv) /= samples.size();
        stddev.getJVector(sv).x = sqrt(stddev.getJVector(sv).x);
        stddev.getJVector(sv).y = sqrt(stddev.getJVector(sv).y);
        stddev.getJVector(sv).z = sqrt(stddev.getJVector(sv).z);
    }
}
