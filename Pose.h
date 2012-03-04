/*!
 *  @file Pose.h
 *  @author Clinton Freeman
 *  @date 5/19/2011
 *
 *  A Pose is composed of some number of PoseSample, which is represented as a
 *  map in which individual samples are identified by whatever string the user
 *  entered as its name when the sample was taken.
 *
 *  In addition to the map of PoseSample, a number of specifically numerated
 *  statistical PoseSample are defined as private member variables: currently
 *  just the mean and standard deviation.
 *
 *  The user can add and remove samples and statistical values are generated
 *  when they do so.
 */

#ifndef POSE_H
#define POSE_H

// Qt
#include <QDataStream>

// local
#include <PoseSample.h>

namespace PSD
{

class Pose
{
public:
    Pose();
    void calcStats();
    QMap<QString, PoseSample> const& getSamples() const;
    PoseSample const getSample(QString const name) const;
    PoseSample const& getMean() const;
    PoseSample const& getStdDev() const;
    bool const isModified() const;
    void addSample(QString const &name, PoseSample const &sample);
    void setSample(QString const &name, PoseSample const &sample);
    void removeSample(QString const &name);
    void clearSamples();
    void setSamples(QMap<QString, PoseSample> const &samples);
    void setModified(bool const bModified);
    void setMean(PoseSample const& mean);
    void setStdDev(PoseSample const& stddev);
private:
    QMap<QString, PoseSample> samples;
    PoseSample mean, stddev; //! @todo rename to tolerance or something
    bool bModified;
};

}

// Debugging
QDebug operator<<(QDebug d, PSD::Pose const &p);

// Serialization
QDataStream& operator<<(QDataStream &out, PSD::Pose const &p);
QDataStream& operator>>(QDataStream &in, PSD::Pose &p);

#endif // POSE_H
