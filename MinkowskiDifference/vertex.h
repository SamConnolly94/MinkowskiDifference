#pragma once

#include "vector.h"

struct Vertex
{
    Vertex(float x, float y, float z);
    Vertex(Vector3 position);
    bool operator==(const Vertex& rhs) const;
    Vertex operator+(const Vertex& rhs) const;
    Vertex operator-(const Vertex& rhs) const;
    Vertex operator*(const float& rhs) const;

    Vector3 m_Position{ 0.0f, 0.0f, 0.0f };
};

