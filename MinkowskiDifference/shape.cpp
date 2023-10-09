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
    const Vector3 dir = point.m_Position.GetNormalised();

    auto result = std::min_element(m_Vertices.begin(), m_Vertices.end(), [&](const Vertex& a, const Vertex& b) {
        return Vector3::DotProd(a.m_Position.GetNormalised(), dir) < Vector3::DotProd(b.m_Position.GetNormalised(), dir);
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
