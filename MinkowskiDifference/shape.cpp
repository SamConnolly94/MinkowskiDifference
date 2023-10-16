#include "shape.h"

#include "shapedebug.h"
#include "vertex.h"

#include <algorithm>
#include <iostream>

CShape::CShape(const std::string& name, const std::initializer_list<Vertex>& initialiserList) : m_Name(name), m_Vertices{ initialiserList }
{
}

CShape::CShape(const std::string& name, const std::vector<Vertex>& vertices) : m_Name(name), m_Vertices{ vertices }
{
}

const std::vector<Vertex>& CShape::GetVertices() const
{
    return m_Vertices;
}

Vertex CShape::FindMostDirectPoint(const Vector3& dir) const
{
    auto result = std::max_element(m_Vertices.begin(), m_Vertices.end(), [&](const Vertex& a, const Vertex& b) {
        // Purposefully not normalising the dot prod here so that it scales with the magnitude of the vector
        return Vector3::DotProd(a.m_Position, dir) < Vector3::DotProd(b.m_Position, dir);
        });
    // TODO: Handle what happens when one isn't found.
    return *(result);
}

Vector3 CShape::GetCentrePoint() const
{
    Vector3 result = Vector3::Zero();
    for (Vertex v : m_Vertices)
    {
        result = result + v.m_Position;
    }

    result = result / static_cast<float>(m_Vertices.size());
    return result;
}

CShape CShape::operator=(const CShape& rhs) const
{
    return CShape(rhs.m_Name, rhs.m_Vertices);
}

std::ostream& operator<<(std::ostream& os, const CShape& shape)
{
    return os << CShapeDebug::GetDebugGrid(shape, shape.GetName());
}

bool CShape::HandleSimplex(std::vector<Vertex> simplex, Vector3& d) const
{
    if (simplex.size() == 2)
    {
        return LineCase(simplex, d);
    }
    return TriangleCase(simplex, d);
}

bool CShape::LineCase(std::vector<Vertex> simplex, Vector3& d) const
{
    // Assuming there's only 2 points in a line case. If you're calling this manually, better make sure too ;p
    Vector3 ab = simplex[0].m_Position - simplex[1].m_Position;
    Vector3 ao = Vector3::Zero() - simplex[1].m_Position;
    Vector3 abNormal = Vector3::CrossProd(ab, Vector3::CrossProd(ao, ab));

    // Setting the direction to our perpendicular vec to the origin. This is passed by ref so will be changed elsewhere.
    d = abNormal.GetNormalised();

    // Only a line, cannot be contained within a simplex
    return false;
}

bool CShape::TriangleCase(std::vector<Vertex> simplex, Vector3& d) const
{
#ifdef _DEBUG
    CShape testTriangle = CShape("Testing Simplex", simplex);
    std::cout << testTriangle << std::endl;
#endif
    Vector3 ab = simplex[1].m_Position - simplex[2].m_Position;
    Vector3 ac = simplex[0].m_Position - simplex[2].m_Position;
    Vector3 ao = Vector3::Zero() - simplex[2].m_Position;

    Vector3 abPerpendicular = Vector3::CrossProd(ac, Vector3::CrossProd(ab, ab)).GetNormalised();
    Vector3 acPerpendicular = Vector3::CrossProd(ab, Vector3::CrossProd(ac, ac)).GetNormalised();

    /*A
    | \
    |   \   * Origin lying somewhere in this region, let's remove C
    |     \
    |       \
    |         \
    C___________ B
    */
    if (Vector3::DotProd(abPerpendicular, ao.GetNormalised()) > 0)
    {
        d = abPerpendicular;
        simplex.erase(simplex.begin() + 2);
        return false;
    }
    /*A
    | \
   *|   \
    |     \
    |       \
    |         \
    C___________ B
   origin lying somewhere over to the left side of AC, pointless having B, let's remove it
   */
    else if (Vector3::DotProd(acPerpendicular, ao.GetNormalised()) > 0)
    {
        d = acPerpendicular;
        simplex.erase(simplex.begin() + 1);
        return false;
    }

    // If neither of the above cases match, we can be assured that the triangle contains the origin
    return true;
}

bool CShape::IntersectsWith(const CShape& other)
{
    const CShape aShape = this->GetVertices().size() > other.GetVertices().size() ? *this : other;
    const CShape bShape = this->GetVertices().size() > other.GetVertices().size() ? other : *this;

    Vector3 d = (bShape.GetCentrePoint() - aShape.GetCentrePoint()).GetNormalised();
    Vertex a1 = aShape.FindMostDirectPoint(d);
    Vertex b1 = bShape.FindMostDirectPoint(Vector3::Zero() - d);
    Vertex s1 = a1 - b1;

    d = (Vector3::Zero() - s1.m_Position).GetNormalised();
    std::vector<Vertex> simplex = { s1 };
    while (true)
    {
        Vertex a2 = aShape.FindMostDirectPoint(d);
        Vertex b2 = bShape.FindMostDirectPoint(Vector3::Zero() - d);
        Vertex s2 = a2 - b2;

#ifdef _DEBUG
        CShape testTriangle = CShape("Support Points", {s1, s2});
        std::cout << testTriangle << std::endl;
#endif
        if (Vector3::DotProd(s2.m_Position.GetNormalised(), d) < 0.0f)
        {
            return false;
        }

        simplex.push_back(s2);
        if (HandleSimplex(simplex, d))
        {
            CShape testTriangle = CShape("Test Triangle", simplex);
#ifdef _DEBUG
            std::cout << testTriangle << std::endl;
#endif
            return true;
        }
    }

    // Still not found

    return false;
}

const std::string& CShape::GetName() const
{
    return m_Name;
}