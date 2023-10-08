#include <iostream>
#include <memory>
#include <vector>

#include "vertex.h"
#include "shape.h"

using namespace std;

void PushVertexIfNotPresent(std::vector<Vertex>& result, float x, float y, float z)
{
    Vertex a{ x, y, z };
    if (std::find(result.begin(), result.end(), a) == result.end())
    {
        result.push_back(a);
    }
}

CShape CalculateMinkowskiDistance(const CShape& a, const CShape& b)
{
    // TODO:
    // We should be able to work out what size this should be.
    // If so, let's change this to a std::array<> instead.
    // At the very least, reserve the memory we'll take up.
    std::vector<Vertex> vertices;

    for (const auto& leftVertex : a.GetVertices())
    {
        for (const auto rightVertex : b.GetVertices())
        {
            float xX = leftVertex.m_X - rightVertex.m_X;
            float xY = leftVertex.m_X - rightVertex.m_Y;
            float xZ = leftVertex.m_X - rightVertex.m_Z;
            PushVertexIfNotPresent(vertices, xX, xY, xZ);

            float yX = leftVertex.m_Y - rightVertex.m_X;
            float yY = leftVertex.m_Y - rightVertex.m_Y;
            float yZ = leftVertex.m_Y - rightVertex.m_Z;
            PushVertexIfNotPresent(vertices, yX, yY, yZ);

            float zX = leftVertex.m_Z - rightVertex.m_X;
            float zY = leftVertex.m_Z - rightVertex.m_Y;
            float zZ = leftVertex.m_Z - rightVertex.m_Z;
            PushVertexIfNotPresent(vertices, zX, zY, zZ);
        }
    }
    
    return CShape{ vertices };
}

int main()
{
    // Not using unique pointers purposefully, we know the size of an instance of CShape is X, we can get a tonne of them on the stack if we want.
    CShape triangle({ Vertex{-5.0f, 5.0f, 0.0f}, Vertex{-5.0f, -5.0f, 0.0f}, Vertex{2.0f, 0.0, 0.0f} });
    CShape square({ Vertex{ 0.0f, 2.0f, 0.0f }, Vertex{ 0.0f, -2.0f, 0.0f }, Vertex{ 2.0f, 2.0, 0.0f }, Vertex{2.0f, -2.0f, 0.0f} });

    CShape minkowskiDistance = CalculateMinkowskiDistance(triangle, square);
    Vertex extremeVertex = minkowskiDistance.FindMostExtremePoint();

    cout << "Done.";

}