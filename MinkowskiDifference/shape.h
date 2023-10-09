#pragma once

#include <vector>
#include <initializer_list>

struct Vertex;

class CShape
{
public:
    CShape(const std::initializer_list<Vertex>& initialiserList);
    CShape(const std::vector<Vertex>& vertices);
    const std::vector<Vertex>& GetVertices() const;

    Vertex FindOppositeSupportPoint(const Vertex& point) const;
    Vertex FindMostExtremePoint();
protected:
    std::vector<Vertex> m_Vertices;
};

