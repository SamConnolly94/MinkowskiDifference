#pragma once

#include <vector>
#include <initializer_list>

#include <ostream>

struct Vertex;

enum class MinkowskiType
{
    Sum,
    Difference
};

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
    Vertex FindMostDirectPoint(const Vertex& point) const;
    Vertex FindOppositeMostPoint(const Vertex& point) const;
    Vertex FindMostExtremePoint();
    CShape CalculateMinkowskiShape(const MinkowskiType& minkowskiType, const CShape& other) const;
    void ReorderPolygon();
protected:
    std::vector<Vertex> m_Vertices;
    std::string m_Name{ "Shape" };
};

