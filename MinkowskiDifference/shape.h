#pragma once

#include <vector>
#include <initializer_list>

#include <ostream>

struct Vertex;
struct Vector3;

class CShape
{
public:
    // Constructors
    CShape(const std::string& name, const std::initializer_list<Vertex>& initialiserList);
    CShape(const std::string& name, const std::vector<Vertex>& vertices);

    // Operators
    CShape operator=(const CShape& rhs) const;
    friend std::ostream& operator<<(std::ostream& os, const CShape& shape);

    // Getters
    const std::vector<Vertex>& GetVertices() const;
    const std::string& GetName() const;

    // Logic
    bool IntersectsWith(const CShape& other);
protected:
    Vertex FindMostDirectPoint(const Vector3& dir) const;
    Vector3 GetCentrePoint() const;
    bool HandleSimplex(std::vector<Vertex> simplex, Vector3& d) const;
    bool LineCase(std::vector<Vertex> simplex, Vector3& d) const;
    bool TriangleCase(std::vector<Vertex> simplex, Vector3& d) const;
protected:
    std::vector<Vertex> m_Vertices;
    std::string m_Name{ "Shape" };
};

