#include "triangle.h"

#include "shapedebug.h"
#include "vertex.h"
#include "vector.h"

float CTriangle::CalculateArea(Vector3 v1, Vector3 v2, Vector3 v3) const
{
    return fabsf((v1.m_X * (v2.m_Y - v3.m_Y) + v2.m_X * (v3.m_Y - v1.m_Y) + v3.m_X * (v1.m_Y - v2.m_Y)) / 2.0f);
}

bool CTriangle::IsPointInTriangle(const Vector3& position) const
{
    float a = CalculateArea(m_Vertices[0].m_Position, m_Vertices[1].m_Position, m_Vertices[2].m_Position);
    float a1 = CalculateArea(position, m_Vertices[1].m_Position, m_Vertices[2].m_Position);
    float a2 = CalculateArea(position, m_Vertices[0].m_Position, m_Vertices[2].m_Position);
    float a3 = CalculateArea(position, m_Vertices[0].m_Position, m_Vertices[1].m_Position);

    return (a == a1 + a2 + a3);
}
