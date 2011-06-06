/*
===============================================================================
File:       Pose.h
Author:     Clinton Freeman
Created:    May 19, 2011
===============================================================================
*/

#ifndef POSE_H
#define POSE_H

// system
#include <string>
#include <map>

// local
#include <PoseSample.h>

class Pose
{
private:
    std::map<std::string, PoseSample> samples;
    PoseSample mean, tolerance, stddev;

public:
    void calculateStatistics();

    inline void addSample(std::string name, PoseSample& sample)
    {
        samples[name] = sample;
    }

    inline PoseSample& getSample(std::string name)
    {
        return samples[name];
    }

    inline void removeSample(std::string name)
    {
        samples.erase(name);
    }

    inline PoseSample& getMean()
    {
        return mean;
    }

    inline PoseSample& getStdDev()
    {
        return stddev;
    }
};

#endif // POSE_H
