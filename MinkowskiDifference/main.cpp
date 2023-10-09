#include <iostream>
#include <memory>
#include <vector>

#include "vertex.h"
#include "shape.h"

using namespace std;

constexpr int k_DebugRowCount = 5;
constexpr int k_DebugColCount = 5;

void OutputShapeDebug(const CShape& shape, std::string title)
{
    cout << endl << "===========================" << endl << title << endl << "===========================" << endl;

    // Output coords above
    for (int x = -k_DebugColCount; x <= k_DebugColCount; x++)
    {
        if (x >= 0)
        {
            cout << " " << x;
        }
        else
        {
            cout << x;
        }
    }
    cout << endl;

    for (int x = -k_DebugColCount; x < k_DebugColCount; x++)
    {
        cout << " _";
    }
    cout << endl;

    for (int y = -k_DebugRowCount; y <= k_DebugRowCount; y++)
    {
        for (int x = -k_DebugColCount; x < k_DebugColCount; x++)
        {
            auto it = std::find_if(shape.GetVertices().begin(), shape.GetVertices().end(), [&](const Vertex& vertex) {
                return static_cast<float>(x) == vertex.m_Position.m_X && static_cast<float>(y) == vertex.m_Position.m_Y;
                });

            if (it != shape.GetVertices().end())
            {
                cout << "|*";
            }
            else
            {
                cout << "|_";
            }
        }

        cout << "| " << y << endl;
    }
    cout << endl;
}

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
            float xX = leftVertex.m_Position.m_X - rightVertex.m_Position.m_X;
            float xY = leftVertex.m_Position.m_X - rightVertex.m_Position.m_Y;
            float xZ = leftVertex.m_Position.m_X - rightVertex.m_Position.m_Z;
            PushVertexIfNotPresent(vertices, xX, xY, xZ);

            float yX = leftVertex.m_Position.m_Y - rightVertex.m_Position.m_X;
            float yY = leftVertex.m_Position.m_Y - rightVertex.m_Position.m_Y;
            float yZ = leftVertex.m_Position.m_Y - rightVertex.m_Position.m_Z;
            PushVertexIfNotPresent(vertices, yX, yY, yZ);

            float zX = leftVertex.m_Position.m_Z - rightVertex.m_Position.m_X;
            float zY = leftVertex.m_Position.m_Z - rightVertex.m_Position.m_Y;
            float zZ = leftVertex.m_Position.m_Z - rightVertex.m_Position.m_Z;
            PushVertexIfNotPresent(vertices, zX, zY, zZ);
        }
    }

    return CShape{ vertices };
}

int main()
{
    // Not using unique pointers purposefully, we know the size of an instance of CShape is X, we can get a tonne of them on the stack if we want.
    CShape triangle({ Vertex{-5.0f, 5.0f, 0.0f}, Vertex{-5.0f, -5.0f, 0.0f}, Vertex{2.0f, 0.0, 0.0f} });
    Vertex triangleSupportPoint = triangle.FindMostExtremePoint();
    OutputShapeDebug(triangle, "Triangle");

    CShape square({ Vertex{ 0.0f, 2.0f, 0.0f }, Vertex{ 0.0f, -2.0f, 0.0f }, Vertex{ 2.0f, 2.0, 0.0f }, Vertex{2.0f, -2.0f, 0.0f} });
    Vertex squareSupportPoint = square.FindOppositeSupportPoint(triangleSupportPoint);
    OutputShapeDebug(square, "Square");

    CShape minkowskiDistance = CalculateMinkowskiDistance(triangle, square);
    Vertex mostExtremeVertex = minkowskiDistance.FindMostExtremePoint();
    OutputShapeDebug(minkowskiDistance, "Minkowski");
}