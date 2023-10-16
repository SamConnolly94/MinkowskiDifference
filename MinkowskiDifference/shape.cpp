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
    return FindOppositeMostPoint(point, dir);
}

Vertex CShape::FindOppositeMostPoint(const Vertex& point, const Vector3 dir) const
{
    auto v = std::max_element(m_Vertices.begin(), m_Vertices.end(), [&](const Vertex& a, const Vertex& b) {
        // Purposefully not normalising the dot prod here so that it scales with the magnitude of the vector
        return Vector3::DotProd(a.m_Position, dir) > Vector3::DotProd(b.m_Position, dir);
        });
    return *v;
}

bool CShape::PassesGjkSanityCheck(const Vector3& pos, const Vector3& pointFacing) const
{
    Vector3 inverseDirToPoint = (Vector3::Zero() - pos).GetNormalised();

    // Sanity check if we got past the origin at this point
    const float dotProd = Vector3::DotProd(inverseDirToPoint, pointFacing);
    if (dotProd < 0.0f)
    {
        // Can't possibly be intersecting because C1's dir from origin does not point in the same direction as C2's dir
        return false;
    }

    return true;
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
        return a.m_Position.m_Y < b.m_Position.m_Y || (yIsIdentical && a.m_Position.m_X < b.m_Position.m_X) || ((yIsIdentical && a.m_Position.m_X == a.m_Position.m_X) && a.m_Position.m_Z < b.m_Position.m_Z);
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
    // Select first point
    Vertex a1 = FindMostExtremePoint();
    Vertex b1 = other.FindOppositeMostPoint(a1);
    Vertex c1 = a1 - b1;

    // Select second point
    Vertex a2 = FindOppositeMostPoint(a1);
    Vertex b2 = other.FindOppositeMostPoint(a2);
    Vertex c2 = a2 - b2;

    if (!PassesGjkSanityCheck(c1.m_Position, c2.m_Position.GetNormalised()))
    {
        return false;
    }

    const float distance = Vector3::Distance(c1.m_Position, c2.m_Position);
    const float halfDistance = distance / 2.0f;

    // Scoped because no need for anyone else to know about variables in this section, probably indicates it should move into it's own function
    Vector3 c1toc2Dir = (Vector3::Zero() - c1.m_Position).GetNormalised();

    // Packing into a vertex to use with  helper functions, it's not really a vertex on the shape
    Vertex halfwayPosition = { c1.m_Position + (c1toc2Dir * halfDistance) };

    Vector3 lineNormal = Vector3{ halfwayPosition.m_Position.m_Y, -halfwayPosition.m_Position.m_X, halfwayPosition.m_Position.m_Z }.GetNormalised();

    // Invert the noraml if it was facing the same direction as the origin
    if (Vector3::DotProd(lineNormal, halfwayPosition.m_Position.GetNormalised()) > 0)
    {
        lineNormal = Vector3::Zero() - lineNormal;
    }

    Vertex a3 = FindOppositeMostPoint(halfwayPosition, lineNormal.GetNormalised());
    Vertex b3 = other.FindOppositeMostPoint(a3);
    Vertex c3 = a3 - b3;

    CTriangle testTriangle = CTriangle("Test Triangle", { c1, c2, c3 });
    if (testTriangle.IsPointInTriangle(Vector3::Zero()))
    {
        std::cout << testTriangle << std::endl << "INTERSECTION FOUND " << std::endl;
        std::cout << "-------------------------------------------" << std::endl;

        return true;
    }

#ifdef _DEBUG
        std::cout << testTriangle << std::endl;
#endif

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
        std::cout << CShape("Negated Shape", bShape.m_Vertices);
    }

    std::vector<Vertex> vertices;
    for (const Vertex vA : aShape.GetVertices())
    {
        // If performing difference, we want the opposite most point.
        Vertex vB = bShape.FindMostDirectPoint(vA);
        vertices.push_back({ vA + vB });
    }

    std::string name = minkowskiType == MinkowskiType::Sum ? "Minkowski Sum" : "Minkowski Difference";

    return CShape{ name, vertices };
}

const std::string& CShape::GetName() const
{
    return m_Name;
}