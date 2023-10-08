#include "vertex.h"

#include <math.h>

Vertex::Vertex(float x, float y, float z) : m_X(x), m_Y(y), m_Z(z)
{
}

float Vertex::Length() const
{   
    return fabsf(m_X) + fabsf(m_Y) + fabsf(m_Z);
}

bool Vertex::operator==(const Vertex& rhs) const
{
    return m_X == rhs.m_X && m_Y == rhs.m_Y && m_Z == rhs.m_Z;
}

bool Vertex::operator>(const Vertex& rhs) const
{
    return this->Length() > rhs.Length();
}

bool Vertex::operator>=(const Vertex& rhs) const
{
    return this->Length() >= rhs.Length();
}

bool Vertex::operator<(const Vertex& rhs) const
{
    return this->Length() < rhs.Length();
}

bool Vertex::operator<=(const Vertex& rhs) const
{
    return this->Length() <= rhs.Length();
}
