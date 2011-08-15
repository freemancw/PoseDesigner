/*
===============================================================================
File:       Pose.h
Author:     Clinton Freeman
Created:    May 19, 2011

Comments:   A Pose is composed of some number of PoseSamples, which is
            represented as a std::map in which individual samples are
            identified by whatever string the user entered as its name when the
            sample was taken.

            In addition to the map of PoseSamples, a number of specifically
            numerated statistical PoseSamples are defined as private member
            variables: currently just the mean and standard deviation.

            The user can add and remove samples and statistical values are
            generated when they do so.

            Bug: Since individual samples are identified by their names we need
            to check for when the user chooses the same name twice.
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
    PoseSample mean, stddev;

public:
    void calculateStatistics();

    // getters
    inline PoseSample& getSample(std::string name) { return samples[name]; }
    inline PoseSample& getMean() { return mean; }
    inline PoseSample& getStdDev() { return stddev; }

    // setters/modifiers
    inline void addSample(std::string name, PoseSample& sample)
    { samples[name] = sample; }
    inline void removeSample(std::string name) { samples.erase(name); }
};

#endif // POSE_H
