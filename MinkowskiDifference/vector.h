#pragma once

struct Vector3
{
    Vector3(float x, float y, float z) : m_X(x), m_Y(y), m_Z(z) {};

    float Length() const;
    Vector3 GetNormalised() const;
    static float DotProd(const Vector3& a, const Vector3& b);
    static float Distance(const Vector3& a, const Vector3& b);

    bool operator==(const Vector3& rhs) const;
    bool operator>(const Vector3& rhs) const;
    bool operator>=(const Vector3& rhs) const;
    bool operator<(const Vector3& rhs) const;
    bool operator<=(const Vector3& rhs) const;
    Vector3 operator-(const Vector3& rhs) const;
    Vector3 operator*(const float x) const;
    Vector3 operator+(const Vector3& rhs) const;

    float m_X{ 0.0f };
    float m_Y{ 0.0f };
    float m_Z{ 0.0f };
};