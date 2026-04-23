#pragma once
#include <cmath>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace boost::multiprecision;
using largeFloat = cpp_dec_float_100;

class Vector
{
public:
    largeFloat x, y, z, magnitude;

    Vector()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->magnitude = 0;
    }

    Vector(glm::vec2 v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = 0;
        this->magnitude = sqrt((x * x) + (y * y) + (z * z));
    }

    Vector(glm::vec3 v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->magnitude = sqrt((x * x) + (y * y) + (z * z));
    }

    Vector(glm::vec4 v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->magnitude = sqrt((x * x) + (y * y) + (z * z));
    }

    Vector(largeFloat x, largeFloat y, largeFloat z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->magnitude = sqrt((x * x) + (y * y) + (z * z));
    }

    Vector(largeFloat x, largeFloat y)
    {
        this->x = x;
        this->y = y;
        this->z = 0;
        this->magnitude = sqrt((x * x) + (y * y) + (z * z));
    }

    Vector(largeFloat xyz)
    {
        this->x = xyz;
        this->y = xyz;
        this->z = xyz;
        this->magnitude = sqrt((x * x) + (y * y) + (z * z));
    }

    glm::vec3 getGLM()
    {
        return glm::vec3(x, y, z);
    }

    void normalize()
    {
        if (magnitude == 0)
            *this = Vector(0);

        this->x /= static_cast<largeFloat>(magnitude);
        this->y /= static_cast<largeFloat>(magnitude);
        this->z /= static_cast<largeFloat>(magnitude);
    }

    Vector normalized()
    {
        if (magnitude == 0)
            return Vector(0);

        return *this / static_cast<largeFloat>(magnitude);
    }

    largeFloat distance(const Vector &other)
    {
        return sqrt(pow(other.x - this->x, 2) + pow(other.y - this->y, 2) + pow(other.z - this->z, 2));
    }

    float dot(const Vector &other)
    {
        return static_cast<float>((x * other.x) + (y * other.y) + (z * other.z));
    }

    Vector cross(const Vector &other)
    {
        return Vector(
            (this->y * other.z) - (this->z * other.y),
            (this->z * other.x) - (this->x * other.z),
            (this->x * other.y) - (this->y * other.x));
    }

    bool operator==(const Vector &other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const Vector &other) const
    {
        return x != other.x || y != other.y || z != other.z;
    }

    Vector operator+(const Vector &other) const
    {
        return Vector(x + other.x, y + other.y, z + other.z);
    }

    Vector operator-(const Vector &other) const
    {
        return Vector(x - other.x, y - other.y, z - other.z);
    }

    Vector operator*(const Vector &other) const
    {
        return Vector(x * other.x, y * other.y, z * other.z);
    }

    Vector operator/(const Vector &other) const
    {
        return Vector(
            (other.x != 0) ? x / other.x : largeFloat(0),
            (other.y != 0) ? y / other.y : largeFloat(0),
            (other.z != 0) ? z / other.z : largeFloat(0));
    }

    void operator+=(const Vector &other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        this->magnitude = sqrt((x * x) + (y * y) + (z * z));
    }

    void operator-=(const Vector &other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        this->magnitude = sqrt((x * x) + (y * y) + (z * z));
    }

    void operator*=(const Vector &other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        this->magnitude = sqrt((x * x) + (y * y) + (z * z));
    }

    void operator/=(const Vector &other)
    {
        x = (other.x != 0) ? x / other.x : largeFloat(0);
        y = (other.y != 0) ? y / other.y : largeFloat(0);
        z = (other.z != 0) ? z / other.z : largeFloat(0);
        this->magnitude = sqrt((x * x) + (y * y) + (z * z));
    }

    Vector operator+(const long double &other) const
    {
        return Vector(x + other, y + other, z + other);
    }

    Vector operator-(const long double &other) const
    {
        return Vector(x - other, y - other, z - other);
    }

    Vector operator*(const long double &other) const
    {
        return Vector(x * other, y * other, z * other);
    }

    Vector operator/(const long double &other) const
    {
        if (other == 0)
        {
            return Vector(0);
        }

        return Vector(x / other, y / other, z / other);
    }

    void operator+=(const long double &other)
    {
        x += other;
        y += other;
        z += other;
        this->magnitude = sqrt((x * x) + (y * y) + (z * z));
    }

    void operator-=(const long double &other)
    {
        x -= other;
        y -= other;
        z -= other;
        this->magnitude = sqrt((x * x) + (y * y) + (z * z));
    }

    void operator*=(const long double &other)
    {
        x *= other;
        y *= other;
        z *= other;
        this->magnitude = sqrt((x * x) + (y * y) + (z * z));
    }

    void operator/=(const long double &other)
    {
        x = (other != 0) ? x / other : largeFloat(0);
        y = (other != 0) ? y / other : largeFloat(0);
        z = (other != 0) ? z / other : largeFloat(0);
        this->magnitude = sqrt((x * x) + (y * y) + (z * z));
    }

    friend std::ostream &operator<<(std::ostream &out, const Vector &vec)
    {
        out << "Vector(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
        return out;
    }
};
