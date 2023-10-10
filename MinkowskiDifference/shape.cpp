#include "shape.h"

#include "triangle.h"
#include "shapedebug.h"
#include "vertex.h"

#include <algorithm>
#include <iostream>

CShape::CShape(const std::string& name, const std::initializer_list<Vertex>& initialiserList) : m_Name(name), m_Vertices{ initialiserList }
{
    ReorderPolygon();
}

CShape::CShape(const std::string& name, const std::vector<Vertex>& vertices) : m_Name(name), m_Vertices{ vertices }
{
    ReorderPolygon();
}

const std::vector<Vertex>& CShape::GetVertices() const
{
    return m_Vertices;
}

Vertex CShape::FindMostDirectPoint(const Vertex& point) const
{
    const Vector3 dir = point.m_Position.GetNormalised();

    auto result = std::max_element(m_Vertices.begin(), m_Vertices.end(), [&](const Vertex& a, const Vertex& b) {
        // Purposefully not normalising the dot prod here so that it scales with the magnitude of the vector
        return Vector3::DotProd(a.m_Position, dir) < Vector3::DotProd(b.m_Position, dir);
        });
    return *(result);
}

Vertex CShape::FindOppositeMostPoint(const Vertex& point) const
{
    const Vector3 dir = point.m_Position.GetNormalised();

    auto result = std::max_element(m_Vertices.begin(), m_Vertices.end(), [&](const Vertex& a, const Vertex& b) {
        // Purposefully not normalising the dot prod here so that it scales with the magnitude of the vector
        return Vector3::DotProd(a.m_Position, dir) > Vector3::DotProd(b.m_Position, dir);
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

void CShape::ReorderPolygon()
{
    std::sort(m_Vertices.begin(), m_Vertices.end(), [](const Vertex a, const Vertex b) {
        bool yIsIdentical = a.m_Position.m_Y == b.m_Position.m_Y;
        bool xIsIdentical = a.m_Position.m_X == b.m_Position.m_X;
        return a.m_Position.m_Y < b.m_Position.m_Y || 
            (yIsIdentical && a.m_Position.m_X < b.m_Position.m_X) || 
            ((yIsIdentical && xIsIdentical) && a.m_Position.m_Z < b.m_Position.m_Z);
        });
}

CShape CShape::operator=(const CShape& rhs) const
{
    return CShape(rhs.m_Name, rhs.m_Vertices);
}

std::ostream& operator<<(std::ostream& os, const CShape& shape)
{
    return os << CShapeDebug::GetDebugGrid(shape, shape.GetName());
}

bool CShape::IntersectsWith(const CShape& other)
{
    CShape minkowskiDifference = CalculateMinkowskiShape(MinkowskiType::Difference, other);
    std::cout << minkowskiDifference << std::endl;

    Vertex supportPointA = minkowskiDifference.FindMostExtremePoint();
    Vertex supportPointB = minkowskiDifference.FindOppositeMostPoint(supportPointA);

    std::cout << CShape("Support Points", { supportPointA, supportPointB }) << std::endl;

    bool aHasMoreVertices = GetVertices().size() > other.GetVertices().size() ? true : false;

    // NOTE:
    // At the moment this is pretty innefficient. There's something called the GJK algorithm, which should
    // drastically improve the search time. Right now I'm just brute forcing it to check every point for 
    // an intersection. Check out https://youtu.be/ajv46BSqcK4 for a good explanation.
    for (Vertex v1 : minkowskiDifference.m_Vertices)
    {
            CTriangle testTriangle = CTriangle("Test Triangle", { v1, supportPointA, supportPointB});
            if (testTriangle.IsPointInTriangle(Vector3::Zero()))
            {
                std::cout << testTriangle << std::endl << "INTERSECTION FOUND " << std::endl;
                std::cout << "-------------------------------------------" << std::endl;

                return true;
            }

#ifdef _DEBUG
            std::cout << testTriangle << std::endl;
#endif
    }

    std::cout << "NO INTERSECTION WAS FOUND." << std::endl << std::endl;
    return false;
}

CShape CShape::CalculateMinkowskiShape(const MinkowskiType& minkowskiType, const CShape& other) const
{
    bool aHasMoreVertices = GetVertices().size() > other.GetVertices().size() ? true : false;

    CShape aShape = aHasMoreVertices ? CShape{ "a", GetVertices() } : CShape{ "b", other.GetVertices() };
    CShape bShape = aHasMoreVertices ? CShape{ "b", other.GetVertices() } : CShape{ "a", GetVertices() };

    // Invert the positions of the shape if performing difference.
    if (minkowskiType == MinkowskiType::Difference)
    {
        for (auto& v : bShape.m_Vertices)
        {
            v.m_Position.m_X = -v.m_Position.m_X;
            v.m_Position.m_Y = -v.m_Position.m_Y;
            v.m_Position.m_Z = -v.m_Position.m_Z;
        }
#ifdef _DEBUG
        std::cout << CShape("Negated Shape", bShape.m_Vertices);
#endif
    }

    std::vector<Vertex> vertices;
    for (const auto& a : aShape.GetVertices())
    {
        for (const auto& b : bShape.GetVertices())
        {
            vertices.push_back(a + b);
        }
    }

    std::string name = minkowskiType == MinkowskiType::Sum ? "Minkowski Sum" : "Minkowski Difference";

    return CShape{ name, vertices };
}

const std::string& CShape::GetName() const
{
    return m_Name;
}