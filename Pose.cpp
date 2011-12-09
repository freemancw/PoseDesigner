/*!
 *  @file Pose.cpp
 *  @author Clinton Freeman
 *  @date 5/19/2011
 *
 *  Pose implementation. See Pose.h for detailed description.
 *
 *  @todo Since individual samples are identified by their names we need to
 *  check for when the user chooses the same name twice.
 */

// system
#include <math.h>

// local
#include <Pose.h>
#include <PoseSample.h>

/*
===============================================================================
Pose Creation
===============================================================================
*/

void Pose::calculateStatistics()
{
    PoseSample max, min;

    for(SkeletonVector sv = NECK_HEAD; sv < SKEL_VEC_MAX; sv++)
    {
        QMap<QString, PoseSample>::iterator iter;

        max.setJCoord(sv, samples.begin().value().getJCoord(sv));
        min.setJCoord(sv, samples.begin().value().getJCoord(sv));
        for(iter = samples.begin(); iter != samples.end(); iter++)
        {
            if(iter.value().getJCoord(sv).phi < min.getJCoord(sv).phi)
                min.setJCoordPhi(sv, iter.value().getJCoord(sv).phi);

            if(iter.value().getJCoord(sv).theta < min.getJCoord(sv).theta)
                min.setJCoordTheta(sv, iter.value().getJCoord(sv).theta);

            if(iter.value().getJCoord(sv).phi > max.getJCoord(sv).phi)
                max.setJCoordPhi(sv, iter.value().getJCoord(sv).phi);

            if(iter.value().getJCoord(sv).theta > max.getJCoord(sv).theta)
                max.setJCoordTheta(sv, iter.value().getJCoord(sv).theta);

            mean.getJCoord(sv) += iter.value().getJCoord(sv);
        }

        mean.getJCoord(sv) /= samples.size();
        stddev.getJCoord(sv).phi = (max.getJCoord(sv).phi - min.getJCoord(sv).phi)*2;
        stddev.getJCoord(sv).theta = (max.getJCoord(sv).theta - min.getJCoord(sv).theta)*2;
    }
}

/*
===============================================================================
Debugging / Unit Tests
===============================================================================
*/

QDebug operator<<(QDebug d, Pose &p)
{
    d << "Mean: ";
    d << p.getMean();
    d << "Standard Deviation: ";
    d << p.getStdDev();
    d << "Samples: ";

    QMap<QString, PoseSample>::iterator iter;
    QMap<QString, PoseSample> samples = p.getSamples();

    for(iter = samples.begin(); iter != samples.end(); ++iter)
    {
        d << iter.key() << " " << iter.value();
    }

    return d;
}

/*
===============================================================================
Object Serialization
===============================================================================
*/

QDataStream &operator<<(QDataStream &out, Pose &p)
{
    out << p.getMean() << p.getStdDev() << p.getSamples();

    return out;
}

QDataStream &operator>>(QDataStream &in, Pose &p)
{
    PoseSample mean, stddev;
    QMap<QString, PoseSample> samples;

    in >> mean >> stddev >> samples;

    p.getMean() = mean;
    p.getStdDev() = stddev;
    p.getSamples() = samples;

    return in;
}
