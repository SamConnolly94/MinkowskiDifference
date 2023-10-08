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

Vertex CShape::FindMostExtremePoint()
{
    auto it = std::max_element(m_Vertices.begin(), m_Vertices.end());
    return *it;
}
