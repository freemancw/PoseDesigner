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

    for(SkeletonVector sv = NECK_HEAD; sv < SKEL_VEC_MAX; sv++)
    {

        mean.getJVector(sv) = stddev.getJVector(sv) = QVector3D();
        QMap<QString, PoseSample>::iterator iter;

        for(iter = samples.begin(); iter != samples.end(); iter++)
            mean.getJVector(sv) += iter.value().getJVector(sv);

        mean.getJVector(sv) /= samples.size();

        for(iter = samples.begin(); iter != samples.end(); iter++)
        {
            stddev.getJVector(sv) += (iter.value().getJVector(sv) - mean.getJVector(sv)) *
                                     (iter.value().getJVector(sv) - mean.getJVector(sv));
        }

        stddev.getJVector(sv) /= samples.size();
        stddev.getJVector(sv).setX(0.5);
        stddev.getJVector(sv).setY(0.5);
        stddev.getJVector(sv).setZ(0.8);
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
