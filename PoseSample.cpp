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

static QVector3D vecFromJoints(XnSkeletonJointPosition to, XnSkeletonJointPosition from)
{
    QVector3D vA(to.position.X, to.position.Y, to.position.Z);
    QVector3D vB(from.position.X, from.position.Y, from.position.Z);

    return vA - vB;
}

static QVector3D createPerpendicular(QVector3D& v)
{
    float min = v.x();
    QVector3D cardinalAxis(1, 0, 0);

    if(v.y() < min)
    {
        min = v.y();
        cardinalAxis = QVector3D(0, 1, 0);
    }

    if(v.z() < min)
    {
        min = v.z();
        cardinalAxis = QVector3D(0, 0, 1);
    }

    return QVector3D::crossProduct(v, cardinalAxis);
}

static QVector3D projVecOnPlane(QVector3D& v, QVector3D& norm)
{
    QVector3D perp1 = createPerpendicular(norm);
    perp1.normalize();
    QVector3D perp2 = QVector3D::crossProduct(norm, perp1);
    perp2.normalize();

    float proj1 = QVector3D::dotProduct(perp1, v);
    float proj2 = QVector3D::dotProduct(perp2, v);
    QVector3D out = (perp1 * proj1) + (perp2 * proj2);
    return out;
}

static float angBetweenVecs(QVector3D& a, QVector3D& b)
{
    return acos(QVector3D::dotProduct(a, b));
}

void PoseSample::calculateCoords()
{
    // "The first principal component u is always aligned with the longer
    // dimension of the torso and we can canonically orient it top-down..."
    //! @todo might want to choose a point between the hips because it might
    //! work better...
    torsoFrame.u = vecFromJoints(jPositions[XN_SKEL_WAIST],
                                jPositions[XN_SKEL_NECK]);
    torsoFrame.u.normalize();

    // "In contrast, the second principal component r, aligned with the line
    // that connects the shoulders... we must rely on the 'left-right' skeleton
    // orientation inferred by the STA..."
    torsoFrame.r = vecFromJoints(jPositions[XN_SKEL_RIGHT_SHOULDER],
                                jPositions[XN_SKEL_LEFT_SHOULDER]);

    torsoFrame.r.normalize();

    // "Finally, the last axis of the orthonormal basis is computed as a
    // cross product of the first two principal components..."
    torsoFrame.t = QVector3D::crossProduct(torsoFrame.u, torsoFrame.r);
    torsoFrame.t.normalize();

    // "We denote all joints adjacent to the torso as first-degree joints -
    // these include elbows, knees, and the head. We represent these points
    // relative to the adjacent joint in the torso in a coordinate system
    // derived from the torso frame."

    //HEAD
    QVector3D head = vecFromJoints(jPositions[XN_SKEL_NECK],
                                   jPositions[XN_SKEL_HEAD]);
    head.normalize();

    // inclination theta is the angle between u and the vector
    jCoords[NECK_HEAD].theta = angBetweenVecs(torsoFrame.u, head);

    // azimuth phi is the angle between r and the projection of the vector
    // onto the plane whose normal is u
    jCoords[NECK_HEAD].phi = angBetweenVecs(torsoFrame.r, projVecOnPlane(head, torsoFrame.u));

    //LEFT ELBOW
    QVector3D lElbow = vecFromJoints(jPositions[XN_SKEL_LEFT_ELBOW],
                                     jPositions[XN_SKEL_LEFT_SHOULDER]);
    lElbow.normalize();

    jCoords[L_SHOULDER_ELBOW].theta = angBetweenVecs(torsoFrame.u, lElbow);
    jCoords[L_SHOULDER_ELBOW].phi = angBetweenVecs(torsoFrame.r, projVecOnPlane(lElbow, torsoFrame.u));

    //LEFT KNEE
    QVector3D lKnee = vecFromJoints(jPositions[XN_SKEL_LEFT_KNEE],
                                    jPositions[XN_SKEL_LEFT_HIP]);
    lKnee.normalize();

    jCoords[L_HIP_KNEE].theta = angBetweenVecs(torsoFrame.u, lElbow);
    jCoords[L_HIP_KNEE].phi = angBetweenVecs(torsoFrame.r, projVecOnPlane(lKnee, torsoFrame.u));

    //RIGHT ELBOW
    QVector3D rElbow = vecFromJoints(jPositions[XN_SKEL_RIGHT_ELBOW],
                                     jPositions[XN_SKEL_RIGHT_SHOULDER]);
    rElbow.normalize();

    jCoords[R_SHOULDER_ELBOW].theta = angBetweenVecs(torsoFrame.u, rElbow);
    jCoords[R_SHOULDER_ELBOW].phi = angBetweenVecs(torsoFrame.r, projVecOnPlane(rElbow, torsoFrame.u));

    //RIGHT KNEE
    QVector3D rKnee = vecFromJoints(jPositions[XN_SKEL_RIGHT_KNEE],
                                    jPositions[XN_SKEL_RIGHT_HIP]);
    rKnee.normalize();

    jCoords[R_HIP_KNEE].theta = angBetweenVecs(torsoFrame.u, rElbow);
    jCoords[R_HIP_KNEE].phi = angBetweenVecs(torsoFrame.r, projVecOnPlane(rKnee, torsoFrame.u));

    //LEFT HAND
    QVector3D lHand = vecFromJoints(jPositions[XN_SKEL_LEFT_HAND],
                                    jPositions[XN_SKEL_LEFT_ELBOW]);
    lHand.normalize();

    jCoords[L_ELBOW_HAND].theta = angBetweenVecs(lElbow, lHand);
    jCoords[L_ELBOW_HAND].phi = angBetweenVecs(projVecOnPlane(torsoFrame.r, lElbow), lHand);

    //LEFT FOOT
    QVector3D lFoot = vecFromJoints(jPositions[XN_SKEL_LEFT_FOOT],
                                    jPositions[XN_SKEL_LEFT_KNEE]);
    lFoot.normalize();

    jCoords[L_KNEE_FOOT].theta = angBetweenVecs(lKnee, lFoot);
    jCoords[L_KNEE_FOOT].phi = angBetweenVecs(projVecOnPlane(torsoFrame.r, lKnee), lFoot);

    //RIGHT HAND
    QVector3D rHand = vecFromJoints(jPositions[XN_SKEL_RIGHT_HAND],
                                    jPositions[XN_SKEL_RIGHT_ELBOW]);
    rHand.normalize();

    jCoords[R_ELBOW_HAND].theta = angBetweenVecs(rElbow, rHand);
    jCoords[R_ELBOW_HAND].phi = angBetweenVecs(projVecOnPlane(torsoFrame.r, rElbow), rHand);

    //RIGHT FOOT
    QVector3D rFoot = vecFromJoints(jPositions[XN_SKEL_RIGHT_FOOT],
                                    jPositions[XN_SKEL_RIGHT_KNEE]);
    rFoot.normalize();

    jCoords[R_KNEE_FOOT].theta = angBetweenVecs(rKnee, rFoot);
    jCoords[R_KNEE_FOOT].phi = angBetweenVecs(projVecOnPlane(torsoFrame.r, rKnee), rFoot);
}

void PoseSample::calculateVectors()
{

}

//XnVector3D
QDebug operator<<(QDebug d, XnVector3D &v)
{
    d << QString("%1, %2, %3").arg(v.X).arg(v.Y).arg(v.Z);
    return d;
}

QDataStream &operator<<(QDataStream &out, const XnVector3D &p)
{
    out << (float)p.X << (float)p.Y << (float)p.Z;
    return out;
}

QDataStream &operator>>(QDataStream &in, XnVector3D &p)
{
    in >> p.X >> p.Y >> p.Z;
    return in;
}

//XnSkeletonJoint
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

QDebug operator<<(QDebug d, XnSkeletonJoint &sj)
{
    d << XnSkeletonJointNames[sj];
    return d;
}

QDataStream &operator<<(QDataStream &out, XnSkeletonJoint &sj)
{
    out << quint32(sj);
    return out;
}

QDataStream &operator>>(QDataStream &in, XnSkeletonJoint &sj)
{
    quint32 sin;
    in >> sin;
    sj = (XnSkeletonJoint)sin;
    return in;
}

//XnSkeletonJointPosition
QDebug operator<<(QDebug d, XnSkeletonJointPosition &sjp)
{
    d << "fConfidence: " << sjp.fConfidence << ", Position: " << sjp.position;
    return d;
}

QDataStream &operator<<(QDataStream &out, const XnSkeletonJointPosition &sjp)
{
    out << sjp.position << (float)sjp.fConfidence;
    return out;
}

QDataStream &operator>>(QDataStream &in, XnSkeletonJointPosition &sjp)
{
    in >> sjp.position >> sjp.fConfidence;
    return in;
}

//PoseSample
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

QDataStream &operator<<(QDataStream &out, const PoseSample &p)
{
    out << p.getName() << p.getImage() << p.getJPositions() << p.getJVectors();
    return out;
}

QDataStream &operator>>(QDataStream &in, PoseSample &p)
{
    QString name;
    QImage image;
    QMap<XnSkeletonJoint, XnSkeletonJointPosition> jPositions;
    QMap<SkeletonVector, QVector3D> jVectors;

    in >> name >> image >> jPositions >> jVectors;

    //qDebug() << name;

    p.setName(name);
    p.setImage(image);
    p.setJPositions(jPositions);
    p.setJVectors(jVectors);

    return in;
}

