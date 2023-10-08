#pragma once
struct Vertex
{
    Vertex(float x, float y, float z);
    float Length() const;
    bool operator==(const Vertex& rhs) const;
    bool operator>(const Vertex& rhs) const;
    bool operator>=(const Vertex& rhs) const;
    bool operator<(const Vertex& rhs) const;
    bool operator<=(const Vertex& rhs) const;

    float m_X{ 0.0f };
    float m_Y{ 0.0f };
    float m_Z{ 0.0f };
};

