/*
===============================================================================
File:		Vector3.h
Author: 	Clinton Freeman
Created: 	May 7, 2011
===============================================================================
*/

#ifndef VECTOR3_H
#define VECTOR3_H

// OpenNI includes
#include <XnCppWrapper.h>

// system includes
#include <assert.h>
#include <math.h>
#include <string>
#include <sstream>

class Vector3
{
public:
        float x, y, z;

        static const Vector3 XAXIS, YAXIS, ZAXIS, ZERO;

        inline Vector3()
        {
                this->x = this->y = this->z = 0.0;
        }

        inline Vector3(float x, float y, float z)
        {
                this->x = x;
                this->y = y;
                this->z = z;
        }

        inline Vector3(XnSkeletonJointPosition& pos)
        {
            x = pos.position.X;
            y = pos.position.Y;
            z = pos.position.Z;
        }

        inline Vector3& operator = (const Vector3& other)
        {
                x = other.x;
                y = other.y;
                z = other.z;

                return *this;
        }

        inline Vector3 operator + (const Vector3& other)
        {
                return Vector3(x + other.x, y + other.y, z + other.z);
        }

        inline Vector3 operator + (const float scalar)
        {
                return Vector3(x + scalar, y + scalar, z + scalar);
        }

        inline Vector3 operator - (const Vector3& other)
        {
                return Vector3(x - other.x, y - other.y, z - other.z);
        }

        inline Vector3 operator - (const float scalar)
        {
                return Vector3(x - scalar, y - scalar, z - scalar);
        }

        // "Hadamard Product"
        inline Vector3 operator * (const Vector3& other)
        {
                return Vector3(x * other.x, y * other.y, z * other.z);
        }

        inline Vector3 operator * (const float scalar)
        {
                return Vector3(x * scalar, y * scalar, z * scalar);
        }

        inline Vector3 operator / (const Vector3& other)
        {
                assert(other.x != 0.0 && other.y != 0.0 && other.z != 0.0);

                return Vector3(x / other.x, y / other.y, z / other.z);
        }

        inline Vector3 operator / (const float scalar)
        {
                assert(scalar != 0.0);

                return Vector3(x / scalar, y / scalar, z / scalar);
        }

        inline Vector3& operator += (const Vector3& other)
        {
                x += other.x;
                y += other.y;
                z += other.z;

                return *this;
        }

        inline Vector3& operator += (const float scalar)
        {
                x += scalar;
                y += scalar;
                z += scalar;

                return *this;
        }

        inline Vector3& operator -= (const Vector3& other)
        {
                x -= other.x;
                y -= other.y;
                z -= other.z;

                return *this;
        }

        inline Vector3& operator -= (const float scalar)
        {
                x -= scalar;
                y -= scalar;
                z -= scalar;

                return *this;
        }

        // "Hadamard Product"
        inline Vector3& operator *= (const Vector3& other)
        {
                x *= other.x;
                y *= other.y;
                z *= other.z;

                return *this;
        }

        inline Vector3& operator *= (const float scalar)
        {
                x *= scalar;
                y *= scalar;
                z *= scalar;

                return *this;
        }

        inline Vector3& operator /= (const Vector3& other)
        {
                x /= other.x;
                y /= other.y;
                z /= other.z;

                return *this;
        }

        inline Vector3& operator /= (const float scalar)
        {
                assert(scalar != 0.0);

                x /= scalar;
                y /= scalar;
                z /= scalar;

                return *this;
        }

        inline bool operator == (const Vector3& other)
        {
                return x == other.x && y == other.y && z == other.z;
        }

        inline bool operator != (const Vector3& other)
        {
                return x != other.x && y != other.y && z != other.z;
        }

        inline float magnitude()
        {
                return sqrt(x*x + y*y + z*z);
        }

        inline float magnitudeSquared()
        {
                return x*x + y*y + z*z;
        }

        inline float dotProduct(const Vector3& other)
        {
                return x * other.x + y * other.y + z * other.z;
        }

        inline Vector3 crossProduct(const Vector3& other)
        {
                return Vector3(y * other.z - z * other.y,
                                           z * other.x - x * other.z,
                                           x * other.y - y * other.x);
        }

        inline void normalize()
        {
                (*this) *= 1.0 / this->magnitude();
        }

        inline std::string toString() const
        {
                std::ostringstream ss;
                ss << x << ", " << y << ", " << z;
                return ss.str();
        }
};

#endif //VECTOR3_H
