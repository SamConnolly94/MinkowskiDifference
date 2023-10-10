#pragma once

#include "shape.h"

#include <initializer_list>
#include <string>
#include <vector>

struct Vertex;
struct Vector3;

class CTriangle : public CShape
{
public:
    CTriangle(const std::string& name, const std::initializer_list<Vertex>& initialiserList) : CShape(name, initialiserList) {};
    CTriangle(const std::string& name, const std::vector<Vertex>& vertices) : CShape(name, vertices) {};

    float CalculateArea(Vector3 v1, Vector3 v2, Vector3 v3) const;
    bool IsPointInTriangle(const Vector3& position) const;
};