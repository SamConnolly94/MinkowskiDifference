#include "vector.h"

#include <math.h>

float Vector3::Length() const
{
    return fabsf(m_X) + fabsf(m_Y) + fabsf(m_Z);
}

Vector3 Vector3::GetNormalised() const
{
    const float len = Length();
    float x = m_X / len;
    float y = m_Y / len;
    float z = m_Z / len;
    return Vector3(x, y, z);
}

float Vector3::DotProd(const Vector3& a, const Vector3& b)
{
    return a.m_X* b.m_X + a.m_Y * b.m_Y + a.m_Z * b.m_Z;
}

float Vector3::Distance(const Vector3& a, const Vector3& b)
{
    float dist = a.Length() - b.Length();
    return fabsf(dist);
}

bool Vector3::operator==(const Vector3& rhs) const
{
    return m_X == rhs.m_X && m_Y == rhs.m_Y && m_Z == rhs.m_Z;
}

bool Vector3::operator>(const Vector3& rhs) const
{
    return Length() > rhs.Length();
}

bool Vector3::operator>=(const Vector3& rhs) const
{
    return Length() >= rhs.Length();
}

bool Vector3::operator<(const Vector3& rhs) const
{
    return Length() < rhs.Length();
}

bool Vector3::operator<=(const Vector3& rhs) const
{
    return Length() <= rhs.Length();
}

Vector3 Vector3::operator-(const Vector3& rhs) const
{
    return Vector3(m_X - rhs.m_X, m_Y - rhs.m_Y, m_Z - rhs.m_Z);
}

Vector3 Vector3::operator*(const float x) const
{
    return Vector3(m_X * x, m_Y * x, m_Z * x);
}

Vector3 Vector3::operator+(const Vector3& rhs) const
{
    return Vector3(m_X + rhs.m_X, m_Y + rhs.m_Y, m_Z + rhs.m_Z);
}
