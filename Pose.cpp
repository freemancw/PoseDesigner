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

using PSD::Pose;
using PSD::PoseSample;

/*
===============================================================================
Constructors/Destructors
===============================================================================
*/

Pose::Pose() : bModified(false)
{
    mean.setName("Mean");
    stddev.setName("Standard Deviation");
}

/*
===============================================================================
Pose Creation
===============================================================================
*/

void Pose::calcStats()
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

            mean.setJCoord(sv, mean.getJCoord(sv) + iter.value().getJCoord(sv));
        }

        mean.setJCoord(sv, mean.getJCoord(sv) / samples.size());

        qreal phiTolerance = (max.getJCoord(sv).phi - min.getJCoord(sv).phi) * 2.0;
        stddev.setJCoordPhi(sv, phiTolerance);
        qreal thetaTolerance = (max.getJCoord(sv).theta - min.getJCoord(sv).theta) * 2.0;
        stddev.setJCoordTheta(sv, thetaTolerance);
    }
}

/*
===============================================================================
Getters/Setters
===============================================================================
*/

QMap<QString, PoseSample> const& Pose::getSamples() const
{
    return samples;
}

PoseSample const Pose::getSample(QString const name) const
{
    return samples[name];
}

PoseSample const& Pose::getMean() const
{
    return mean;
}

PoseSample const& Pose::getStdDev() const
{
    return stddev;
}

bool const Pose::isModified() const
{
    return bModified;
}

void Pose::addSample(QString const &name, PoseSample const &sample)
{
    samples[name] = sample;
}

void Pose::setSample(QString const &name, PoseSample const &sample)
{
    samples[name] = sample;
}

void Pose::removeSample(QString const &name)
{
    samples.remove(name);
}

void Pose::clearSamples()
{
    samples.clear();
}

void Pose::setSamples(QMap<QString, PoseSample> const &samples)
{
    this->samples = samples;
}

void Pose::setModified(bool const bModified)
{
    this->bModified = bModified;
}

void Pose::setMean(PoseSample const& mean)
{
    this->mean = mean;
}

void Pose::setStdDev(PoseSample const& stddev)
{
    this->stddev = stddev;
}

/*
===============================================================================
Object Serialization
===============================================================================
*/

QDataStream& operator<<(QDataStream &out, Pose const &p)
{
    out << p.getMean() << p.getStdDev() << p.getSamples();
    return out;
}

QDataStream& operator>>(QDataStream &in, Pose &p)
{
    PoseSample mean, stddev;
    QMap<QString, PoseSample> samples;

    in >> mean >> stddev >> samples;

    p.setMean(mean);
    p.setStdDev(stddev);
    p.setSamples(samples);

    return in;
}

/*
===============================================================================
Debugging
===============================================================================
*/

QDebug operator<<(QDebug d, Pose const &p)
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
