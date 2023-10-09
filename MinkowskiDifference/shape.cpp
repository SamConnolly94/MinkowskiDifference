#include "shape.h"

#include "vertex.h"

#include <algorithm>

CShape::CShape(const std::initializer_list<Vertex>& initialiserList) : m_Vertices{ initialiserList }
{
}

CShape::CShape(const std::vector<Vertex>& vertices) : m_Vertices{ vertices }
{
}

const std::vector<Vertex>& CShape::GetVertices() const
{
    return m_Vertices;
}

Vertex CShape::FindOppositeSupportPoint(const Vertex& point) const
{
    float dotProd = 1.0f;
    Vector3 dir = point.m_Position.GetNormalised();

    auto result = std::min_element(m_Vertices.begin(), m_Vertices.end(), [&](const Vertex& a, const Vertex& b) {
        float newDotProd = Vector3::DotProd(a.m_Position.GetNormalised(), dir);
        if (newDotProd < dotProd)
        {
            dotProd = newDotProd;
            return true;
        }
        return false;
        });
    return *(result);
}

Vertex CShape::FindMostExtremePoint()
{
    auto it = std::max_element(m_Vertices.begin(), m_Vertices.end(), [&](const Vertex& a, const Vertex& b) {
        return a.m_Position.Length() < b.m_Position.Length();
        });
    return *it;
}
