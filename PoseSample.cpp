/*!
 *  @file PoseSample.cpp
 *  @author Clinton Freeman
 *  @date 5/16/2011
 *
 *  PoseSample implementation. See PoseSample.h for a detailed description.
 */

// Qt
#include <QVector3D>

// OpenNI
#include <XnCppWrapper.h>

// local
#include <PoseSample.h>
#include <ALGLIB/dataanalysis.h>

// system
#include <assert.h>
#include <math.h>

static QVector3D vecFromJoints(XnSkeletonJointPosition const &to,
                               XnSkeletonJointPosition const &from);
static QVector3D createPerpVec(QVector3D const &v);
static QVector3D projVecOnPlane(QVector3D const &v, QVector3D const &norm);
static qreal angBetweenVecs(QVector3D const &a, QVector3D const &b);

using PSD::PoseSample;
using PSD::SCoord;

/*
===============================================================================
Constructors/Destructors
===============================================================================
*/

PoseSample::PoseSample() {}

/*
===============================================================================
Sample Creation
===============================================================================
*/

void PoseSample::calcCoords()
{
    //! @todo this basis is supposed to be orthonormal in order to calculate
    //! euler angles relative to kinect's basis
    // calculate torso frame
    // "u is always aligned with the longer dimension of the torso,
    // and we can canonically orient it (top-down)"
    torsoFrame.u = vecFromJoints(jPositions[XN_SKEL_WAIST],
                                 jPositions[XN_SKEL_NECK]);
    torsoFrame.u.normalize();

    torsoFrame.r = vecFromJoints(jPositions[XN_SKEL_RIGHT_SHOULDER],
                                 jPositions[XN_SKEL_LEFT_SHOULDER]);
    torsoFrame.r.normalize();

    torsoFrame.t = QVector3D::crossProduct(torsoFrame.u, torsoFrame.r);
    torsoFrame.t.normalize();

    // calculate first degree joints

    // HEAD
    QVector3D head = vecFromJoints(jPositions[XN_SKEL_NECK],
                                   jPositions[XN_SKEL_HEAD]);
    head.normalize();

    // inclination theta is the angle between u and the vector
    jCoords[NECK_HEAD].theta
            = angBetweenVecs(torsoFrame.u, head);

    // azimuth phi is the angle between r and the projection of the vector
    // onto the plane whose normal is u
    jCoords[NECK_HEAD].phi
            = angBetweenVecs(torsoFrame.r,
                             projVecOnPlane(head, torsoFrame.u));

    // LEFT ELBOW
    QVector3D lElbow = vecFromJoints(jPositions[XN_SKEL_LEFT_ELBOW],
                                     jPositions[XN_SKEL_LEFT_SHOULDER]);
    lElbow.normalize();

    jCoords[L_SHOULDER_ELBOW].theta
            = angBetweenVecs(torsoFrame.u, lElbow);
    jCoords[L_SHOULDER_ELBOW].phi
            = angBetweenVecs(torsoFrame.r,
                             projVecOnPlane(lElbow, torsoFrame.u));

    // LEFT KNEE
    QVector3D lKnee = vecFromJoints(jPositions[XN_SKEL_LEFT_KNEE],
                                    jPositions[XN_SKEL_LEFT_HIP]);
    lKnee.normalize();

    jCoords[L_HIP_KNEE].theta
            = angBetweenVecs(torsoFrame.u, lKnee);
    jCoords[L_HIP_KNEE].phi
            = angBetweenVecs(torsoFrame.r,
                             projVecOnPlane(lKnee, torsoFrame.u));

    // RIGHT ELBOW
    QVector3D rElbow = vecFromJoints(jPositions[XN_SKEL_RIGHT_ELBOW],
                                     jPositions[XN_SKEL_RIGHT_SHOULDER]);
    rElbow.normalize();

    jCoords[R_SHOULDER_ELBOW].theta
            = angBetweenVecs(torsoFrame.u, rElbow);
    jCoords[R_SHOULDER_ELBOW].phi
            = angBetweenVecs(torsoFrame.r,
                             projVecOnPlane(rElbow, torsoFrame.u));

    // RIGHT KNEE
    QVector3D rKnee = vecFromJoints(jPositions[XN_SKEL_RIGHT_KNEE],
                                    jPositions[XN_SKEL_RIGHT_HIP]);
    rKnee.normalize();

    jCoords[R_HIP_KNEE].theta
            = angBetweenVecs(torsoFrame.u, rKnee);
    jCoords[R_HIP_KNEE].phi
            = angBetweenVecs(torsoFrame.r,
                             projVecOnPlane(rKnee, torsoFrame.u));

    // calculate second degree joints

    // LEFT HAND
    QVector3D lHand = vecFromJoints(jPositions[XN_SKEL_LEFT_HAND],
                                    jPositions[XN_SKEL_LEFT_ELBOW]);
    lHand.normalize();

    jCoords[L_ELBOW_HAND].theta
            = angBetweenVecs(lElbow, lHand);
    jCoords[L_ELBOW_HAND].phi
            = angBetweenVecs(projVecOnPlane(torsoFrame.r, lElbow),
                             projVecOnPlane(lHand, lElbow));

    // LEFT FOOT
    QVector3D lFoot = vecFromJoints(jPositions[XN_SKEL_LEFT_FOOT],
                                    jPositions[XN_SKEL_LEFT_KNEE]);
    lFoot.normalize();

    jCoords[L_KNEE_FOOT].theta
            = angBetweenVecs(lKnee, lFoot);
    jCoords[L_KNEE_FOOT].phi
            = angBetweenVecs(projVecOnPlane(torsoFrame.r, lKnee),
                             projVecOnPlane(lFoot, lKnee));

    // RIGHT HAND
    QVector3D rHand = vecFromJoints(jPositions[XN_SKEL_RIGHT_HAND],
                                    jPositions[XN_SKEL_RIGHT_ELBOW]);
    rHand.normalize();

    jCoords[R_ELBOW_HAND].theta
            = angBetweenVecs(rElbow, rHand);
    jCoords[R_ELBOW_HAND].phi
            = angBetweenVecs(projVecOnPlane(torsoFrame.r, rElbow),
                             projVecOnPlane(rHand, rElbow));

    // RIGHT FOOT
    QVector3D rFoot = vecFromJoints(jPositions[XN_SKEL_RIGHT_FOOT],
                                    jPositions[XN_SKEL_RIGHT_KNEE]);
    rFoot.normalize();

    jCoords[R_KNEE_FOOT].theta
            = angBetweenVecs(rKnee, rFoot);
    jCoords[R_KNEE_FOOT].phi
            = angBetweenVecs(projVecOnPlane(torsoFrame.r, rKnee),
                             projVecOnPlane(rFoot, rKnee));
}

/*
===============================================================================
Getters/Setters
===============================================================================
*/

QString const& PoseSample::getName() const
{
    return name;
}

QImage const& PoseSample::getImage() const
{
    return image;
}

QMap<XnSkeletonJoint, XnSkeletonJointPosition> const& PoseSample::getJPositions() const
{
    return jPositions;
}

QMap<XnSkeletonJoint, XnSkeletonJointPosition>& PoseSample::getJPositions_nc()
{
    return jPositions;
}

XnSkeletonJointPosition const PoseSample::getJPosition(XnSkeletonJoint const sj) const
{
    return jPositions[sj];
}

QMap<SkeletonVector, SCoord> const& PoseSample::getJCoords() const
{
    return jCoords;
}

SCoord const PoseSample::getJCoord(SkeletonVector const sv) const
{
    return jCoords[sv];
}

void PoseSample::setName(QString const &name)
{
    this->name = name;
}

void PoseSample::setImage(QImage const &image)
{
    this->image = image;
}

void PoseSample::setJPositions(QMap<XnSkeletonJoint, XnSkeletonJointPosition> const &jPositions)
{
    this->jPositions = jPositions;
}

void PoseSample::setJPosition(XnSkeletonJoint const sj, XnSkeletonJointPosition const &jp)
{
    jPositions[sj] = jp;
}

void PoseSample::setJCoords(QMap<SkeletonVector, SCoord> const &jCoords)
{
    this->jCoords = jCoords;
}

void PoseSample::setJCoord(SkeletonVector const sv, SCoord const &c)
{
    jCoords[sv] = c;
}

void PoseSample::setJCoordPhi(SkeletonVector const sv, qreal const phi)
{
    jCoords[sv].phi = phi;
}

void PoseSample::setJCoordTheta(SkeletonVector const sv, qreal const theta)
{
    jCoords[sv].theta = theta;
}

/*
===============================================================================
Object Serialization
===============================================================================
*/

QDataStream& operator<<(QDataStream &out, XnVector3D const &p)
{
    out << static_cast<float>(p.X) << static_cast<float>(p.Y)
        << static_cast<float>(p.Z);
    return out;
}

QDataStream& operator>>(QDataStream &in, XnVector3D &p)
{
    in >> p.X >> p.Y >> p.Z;
    return in;
}

QDataStream& operator<<(QDataStream &out, XnSkeletonJoint const &sj)
{
    out << quint32(sj);
    return out;
}

QDataStream& operator>>(QDataStream &in, XnSkeletonJoint &sj)
{
    quint32 sin;
    in >> sin;
    sj = static_cast<XnSkeletonJoint>(sin);
    return in;
}

QDataStream& operator<<(QDataStream &out, XnSkeletonJointPosition const &sjp)
{
    out << sjp.position << static_cast<float>(sjp.fConfidence);
    return out;
}

QDataStream& operator>>(QDataStream &in, XnSkeletonJointPosition &sjp)
{
    in >> sjp.position >> sjp.fConfidence;
    return in;
}

QDataStream& operator<<(QDataStream &out, PoseSample const &p)
{
    out << p.getName() << p.getImage() << p.getJPositions() << p.getJCoords();
    return out;
}

QDataStream& operator>>(QDataStream &in, PoseSample &p)
{
    QString name;
    QImage image;
    QMap<XnSkeletonJoint, XnSkeletonJointPosition> jPositions;
    QMap<SkeletonVector, SCoord> jCoords;

    in >> name >> image >> jPositions >> jCoords;

    p.setName(name);
    p.setImage(image);
    p.setJPositions(jPositions);
    p.setJCoords(jCoords);

    return in;
}

/*
===============================================================================
Debugging
===============================================================================
*/

static const QString XnSkeletonJointNames[] =
{
    "XN_SKEL_HEAD",
    "XN_SKEL_NECK",
    "XN_SKEL_TORSO",
    "XN_SKEL_WAIST",

    "XN_SKEL_LEFT_COLLAR",
    "XN_SKEL_LEFT_SHOULDER",
    "XN_SKEL_LEFT_ELBOW",
    "XN_SKEL_LEFT_WRIST",
    "XN_SKEL_LEFT_HAND",
    "XN_SKEL_LEFT_FINGERTIP",

    "XN_SKEL_RIGHT_COLLAR",
    "XN_SKEL_RIGHT_SHOULDER",
    "XN_SKEL_RIGHT_ELBOW",
    "XN_SKEL_RIGHT_WRIST",
    "XN_SKEL_RIGHT_HAND",
    "XN_SKEL_RIGHT_FINGERTIP",

    "XN_SKEL_LEFT_HIP",
    "XN_SKEL_LEFT_KNEE",
    "XN_SKEL_LEFT_ANKLE",
    "XN_SKEL_LEFT_FOOT",

    "XN_SKEL_RIGHT_HIP",
    "XN_SKEL_RIGHT_KNEE",
    "XN_SKEL_RIGHT_ANKLE",
    "XN_SKEL_RIGHT_FOOT"
};

QDebug operator<<(QDebug d, XnVector3D const &v)
{
    d << QString("%1, %2, %3").arg(v.X).arg(v.Y).arg(v.Z);
    return d;
}

QDebug operator<<(QDebug d, XnSkeletonJoint const &sj)
{
    d << XnSkeletonJointNames[sj];
    return d;
}

QDebug operator<<(QDebug d, XnSkeletonJointPosition const &jp)
{
    d << "fConfidence: " << jp.fConfidence << ", Position: " << jp.position;
    return d;
}

QDebug operator<<(QDebug d, PoseSample const &p)
{
    d << "Name: " << p.getName();

    QMap<XnSkeletonJoint, XnSkeletonJointPosition>::iterator pIter;
    QMap<XnSkeletonJoint, XnSkeletonJointPosition> jPositions = p.getJPositions();

    for(pIter = jPositions.begin(); pIter != jPositions.end(); ++pIter)
    {
        d << pIter.key() << " " << pIter.value();
    }

    return d;
}

/*
===============================================================================
Convenience Functions
===============================================================================
*/

static QVector3D vecFromJoints(XnSkeletonJointPosition const &to,
                               XnSkeletonJointPosition const &from)
{
    QVector3D vA(to.position.X, to.position.Y, to.position.Z);
    QVector3D vB(from.position.X, from.position.Y, from.position.Z);

    return vA - vB;
}

static QVector3D createPerpVec(QVector3D const &v)
{
    // find out in which axial direction v's magnitude is least
    float min = fabs(v.x());
    QVector3D cardinalAxis(1.0, 0.0, 0.0);

    if(fabs(v.y()) < min)
    {
        min = v.y();
        cardinalAxis = QVector3D(0.0, 1.0, 0.0);
    }

    if(fabs(v.z()) < min)
    {
        min = v.z();
        cardinalAxis = QVector3D(0.0, 0.0, 1.0);
    }

    return QVector3D::crossProduct(v, cardinalAxis);
}

static QVector3D projVecOnPlane(QVector3D const &v, QVector3D const &norm)
{
    QVector3D nNorm = norm;
    nNorm.normalize();

    qreal nProj = QVector3D::dotProduct(nNorm, v);
    return v - (nNorm * nProj);
}

static qreal angBetweenVecs(QVector3D const &a, QVector3D const &b)
{
    QVector3D aNorm = a; aNorm.normalize();
    QVector3D bNorm = b; bNorm.normalize();
    //! @todo is there an acos for qreal from qt?
    return acos(QVector3D::dotProduct(aNorm, bNorm));
}

