#include "vertex.h"

#include <math.h>

Vertex::Vertex(float x, float y, float z)
{
    m_Position = Vector3(x, y, z);
}

Vertex::Vertex(Vector3 position)
{
    m_Position = position;
}

bool Vertex::operator==(const Vertex& rhs) const
{
    return m_Position == rhs.m_Position;
}