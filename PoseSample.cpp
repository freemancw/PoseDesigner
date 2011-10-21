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

// system
#include <assert.h>
#include <math.h>

static QVector3D vecFromJoints(XnSkeletonJointPosition,
                               XnSkeletonJointPosition);
static QVector3D createPerpVec(const QVector3D&);
static QVector3D projVecOnPlane(const QVector3D&, const QVector3D&);
static float angBetweenVecs(QVector3D const&, QVector3D const&);

/*
===============================================================================
Sample Creation
===============================================================================
*/

void PoseSample::calculateCoords()
{
    // calculate torso frame
    torsoFrame.u = vecFromJoints(jPositions[XN_SKEL_WAIST],
                                jPositions[XN_SKEL_NECK]);
    torsoFrame.u.normalize();

    torsoFrame.r = vecFromJoints(jPositions[XN_SKEL_RIGHT_SHOULDER],
                                jPositions[XN_SKEL_LEFT_SHOULDER]);
    torsoFrame.r.normalize();

    torsoFrame.t = QVector3D::crossProduct(torsoFrame.u, torsoFrame.r);
    torsoFrame.t.normalize();

    // calculate first degree joints

    //HEAD
    QVector3D head = vecFromJoints(jPositions[XN_SKEL_NECK],
                                   jPositions[XN_SKEL_HEAD]);
    head.normalize();

    // inclination theta is the angle between u and the vector
    jCoords[NECK_HEAD].theta
            = angBetweenVecs(torsoFrame.u, head);

    // azimuth phi is the angle between r and the projection of the vector
    // onto the plane whose normal is u
    jCoords[NECK_HEAD].phi
            = angBetweenVecs(torsoFrame.r, projVecOnPlane(head, torsoFrame.u));

    //LEFT ELBOW
    QVector3D lElbow = vecFromJoints(jPositions[XN_SKEL_LEFT_ELBOW],
                                     jPositions[XN_SKEL_LEFT_SHOULDER]);
    lElbow.normalize();

    jCoords[L_SHOULDER_ELBOW].theta
            = angBetweenVecs(torsoFrame.u, lElbow);
    jCoords[L_SHOULDER_ELBOW].phi
            = angBetweenVecs(torsoFrame.r, projVecOnPlane(lElbow, torsoFrame.u));

    //LEFT KNEE
    QVector3D lKnee = vecFromJoints(jPositions[XN_SKEL_LEFT_KNEE],
                                    jPositions[XN_SKEL_LEFT_HIP]);
    lKnee.normalize();

    jCoords[L_HIP_KNEE].theta
            = angBetweenVecs(torsoFrame.u, lKnee);
    jCoords[L_HIP_KNEE].phi
            = angBetweenVecs(torsoFrame.r, projVecOnPlane(lKnee, torsoFrame.u));

    //RIGHT ELBOW
    QVector3D rElbow = vecFromJoints(jPositions[XN_SKEL_RIGHT_ELBOW],
                                     jPositions[XN_SKEL_RIGHT_SHOULDER]);
    rElbow.normalize();

    jCoords[R_SHOULDER_ELBOW].theta
            = angBetweenVecs(torsoFrame.u, rElbow);
    jCoords[R_SHOULDER_ELBOW].phi
            = angBetweenVecs(torsoFrame.r, projVecOnPlane(rElbow, torsoFrame.u));

    //RIGHT KNEE
    QVector3D rKnee = vecFromJoints(jPositions[XN_SKEL_RIGHT_KNEE],
                                    jPositions[XN_SKEL_RIGHT_HIP]);
    rKnee.normalize();

    jCoords[R_HIP_KNEE].theta
            = angBetweenVecs(torsoFrame.u, rKnee);
    jCoords[R_HIP_KNEE].phi
            = angBetweenVecs(torsoFrame.r, projVecOnPlane(rKnee, torsoFrame.u));

    // calculate second degree joints

    //LEFT HAND
    QVector3D lHand = vecFromJoints(jPositions[XN_SKEL_LEFT_HAND],
                                    jPositions[XN_SKEL_LEFT_ELBOW]);
    lHand.normalize();

    jCoords[L_ELBOW_HAND].theta
            = angBetweenVecs(lElbow, lHand);
    jCoords[L_ELBOW_HAND].phi
            = angBetweenVecs(projVecOnPlane(torsoFrame.r, lElbow), lHand);

    //LEFT FOOT
    QVector3D lFoot = vecFromJoints(jPositions[XN_SKEL_LEFT_FOOT],
                                    jPositions[XN_SKEL_LEFT_KNEE]);
    lFoot.normalize();

    jCoords[L_KNEE_FOOT].theta
            = angBetweenVecs(lKnee, lFoot);
    jCoords[L_KNEE_FOOT].phi
            = angBetweenVecs(projVecOnPlane(torsoFrame.r, lKnee), lFoot);

    //RIGHT HAND
    QVector3D rHand = vecFromJoints(jPositions[XN_SKEL_RIGHT_HAND],
                                    jPositions[XN_SKEL_RIGHT_ELBOW]);
    rHand.normalize();

    jCoords[R_ELBOW_HAND].theta
            = angBetweenVecs(rElbow, rHand);
    jCoords[R_ELBOW_HAND].phi
            = angBetweenVecs(projVecOnPlane(torsoFrame.r, rElbow), rHand);

    //RIGHT FOOT
    QVector3D rFoot = vecFromJoints(jPositions[XN_SKEL_RIGHT_FOOT],
                                    jPositions[XN_SKEL_RIGHT_KNEE]);
    rFoot.normalize();

    jCoords[R_KNEE_FOOT].theta
            = angBetweenVecs(rKnee, rFoot);
    jCoords[R_KNEE_FOOT].phi
            = angBetweenVecs(projVecOnPlane(torsoFrame.r, rKnee), rFoot);
}

void PoseSample::calculateVectors(){}

/*
===============================================================================
Debugging / Unit Tests
===============================================================================
*/

void PoseSample::unitTest()
{
    float const eps = 0.001;

    assert(angBetweenVecs(QVector3D(1.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0))
           <= (M_PI/2.0 + eps));
    assert(angBetweenVecs(QVector3D(1.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0))
           >= (M_PI/2.0 - eps));
    assert(angBetweenVecs(QVector3D(1.0, 0.0, 0.0), QVector3D(1.0, 1.0, 0.0))
           <= (M_PI/4.0 + eps));
    assert(angBetweenVecs(QVector3D(1.0, 0.0, 0.0), QVector3D(1.0, 1.0, 0.0))
           >= (M_PI/4.0 - eps));
    assert(angBetweenVecs(QVector3D(0.0, 1.0, 0.0), QVector3D(1.0, 1.0, 0.0))
           <= (M_PI/4.0 + eps));
    assert(angBetweenVecs(QVector3D(0.0, 1.0, 0.0), QVector3D(1.0, 1.0, 0.0))
           >= (M_PI/4.0 - eps));
    assert(angBetweenVecs(QVector3D(1.0, 0.0, 0.0), QVector3D(-1.0, 0.0, 0.0))
           <= (M_PI + eps));
    assert(angBetweenVecs(QVector3D(1.0, 0.0, 0.0), QVector3D(-1.0, 0.0, 0.0))
           >= (M_PI - eps));
}

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

QDebug operator<<(QDebug d, XnVector3D &v)
{
    d << QString("%1, %2, %3").arg(v.X).arg(v.Y).arg(v.Z);
    return d;
}

QDebug operator<<(QDebug d, XnSkeletonJoint &sj)
{
    d << XnSkeletonJointNames[sj];
    return d;
}

QDebug operator<<(QDebug d, XnSkeletonJointPosition &sjp)
{
    d << "fConfidence: " << sjp.fConfidence << ", Position: " << sjp.position;
    return d;
}

QDebug operator<<(QDebug d, PoseSample &p)
{
    d << "Name: " << p.getName();

    QMap<XnSkeletonJoint, XnSkeletonJointPosition>::iterator pIter;
    QMap<XnSkeletonJoint, XnSkeletonJointPosition> jPositions = p.getJPositions();

    for(pIter = jPositions.begin(); pIter != jPositions.end(); ++pIter)
    {
        d << pIter.key() << " " << pIter.value();
    }

    QMap<SkeletonVector, QVector3D>::iterator vIter;
    QMap<SkeletonVector, QVector3D> jVectors = p.getJVectors();

    for(vIter = jVectors.begin(); vIter != jVectors.end(); ++vIter)
    {
        d << vIter.key() << " " << vIter.value();
    }

    return d;
}

/*
===============================================================================
Object Serialization
===============================================================================
*/

/*!
 * XnVector3D out
 */
QDataStream &operator<<(QDataStream &out, const XnVector3D &p)
{
    out << (float)p.X << (float)p.Y << (float)p.Z;
    return out;
}

/*!
 * XnVector3D in
 */
QDataStream &operator>>(QDataStream &in, XnVector3D &p)
{
    in >> p.X >> p.Y >> p.Z;
    return in;
}

/*!
 * XnSkeletonJoint out
 */
QDataStream &operator<<(QDataStream &out, XnSkeletonJoint &sj)
{
    out << quint32(sj);
    return out;
}

/*!
 * XnSkeletonJoint in
 */
QDataStream &operator>>(QDataStream &in, XnSkeletonJoint &sj)
{
    quint32 sin;
    in >> sin;
    sj = (XnSkeletonJoint)sin;
    return in;
}

/*!
 * XnSkeletonJointPosition out
 */
QDataStream &operator<<(QDataStream &out, const XnSkeletonJointPosition &sjp)
{
    out << sjp.position << (float)sjp.fConfidence;
    return out;
}

/*!
 * XnSkeletonJointPosition in
 */
QDataStream &operator>>(QDataStream &in, XnSkeletonJointPosition &sjp)
{
    in >> sjp.position >> sjp.fConfidence;
    return in;
}

/*!
 * SphericalCoords out
 */
QDataStream &operator <<(QDataStream &out, const SphericalCoords &s)
{
    out << (float)s.phi << (float)s.theta;
    return out;
}

/*!
 * SphericalCoords in
 */
QDataStream &operator >>(QDataStream &in, SphericalCoords &s)
{
    in >> s.phi >> s.theta;
    return in;
}

/*!
 * PoseSample out
 */
QDataStream &operator<<(QDataStream &out, const PoseSample &p)
{
    out << p.getName() << p.getImage() << p.getJPositions() << p.getJCoords();
    return out;
}

/*!
 * PoseSample in
 */
QDataStream &operator>>(QDataStream &in, PoseSample &p)
{
    QString name;
    QImage image;
    QMap<XnSkeletonJoint, XnSkeletonJointPosition> jPositions;
    QMap<SkeletonVector, SphericalCoords> jCoords;

    in >> name >> image >> jPositions >> jCoords;

    p.setName(name);
    p.setImage(image);
    p.setJPositions(jPositions);
    p.setJCoords(jCoords);

    return in;
}

/*
===============================================================================
Convenience Functions
===============================================================================
*/

static QVector3D vecFromJoints(XnSkeletonJointPosition to,
                               XnSkeletonJointPosition from)
{
    QVector3D vA(to.position.X, to.position.Y, to.position.Z);
    QVector3D vB(from.position.X, from.position.Y, from.position.Z);

    return vA - vB;
}

static QVector3D createPerpVec(QVector3D const& v)
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

static QVector3D projVecOnPlane(QVector3D const& v, QVector3D const& norm)
{
    QVector3D nNorm = norm;
    nNorm.normalize();

    float nProj = QVector3D::dotProduct(nNorm, v);
    return v - (nNorm * nProj);
}

static float angBetweenVecs(QVector3D const& a, QVector3D const& b)
{
    QVector3D aNorm = a; aNorm.normalize();
    QVector3D bNorm = b; bNorm.normalize();
    return acos(QVector3D::dotProduct(aNorm, bNorm));
}

