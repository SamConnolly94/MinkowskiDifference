#include <iostream>
#include <memory>
#include <vector>

#include "vertex.h"
#include "shape.h"

using namespace std;

constexpr int k_DebugRowCount = 5;
constexpr int k_DebugColCount = 5;

float area(Vector3 v1, Vector3 v2, Vector3 v3)
{
    return fabsf((v1.m_X * (v2.m_Y - v3.m_Y) + v2.m_X * (v3.m_Y - v1.m_Y) + v3.m_X * (v1.m_Y - v2.m_Y)) / 2.0f);
}

bool IsPointInTriangle(const Vector3& position, const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
    float a = area(v1.m_Position, v2.m_Position, v3.m_Position);
    float a1 = area(position, v2.m_Position, v3.m_Position);
    float a2 = area(position, v1.m_Position, v3.m_Position);
    float a3 = area(position, v1.m_Position, v2.m_Position);

    return (a == a1 + a2 + a3);
}

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

void PushVertexIfNotPresent(std::vector<Vertex>& result, Vector3 pos)
{
    Vertex a{ pos.m_X, pos.m_Y, pos.m_Z };
    //if (std::find(result.begin(), result.end(), a) == result.end())
    {
        result.push_back(a);
    }
}

CShape CalculateMinkowskiDifference(const CShape& a, const CShape& b)
{
    // TODO:
    // We should be able to work out what size this should be.
    // If so, let's change this to a std::array<> instead.
    // At the very least, reserve the memory we'll take up.
    std::vector<Vertex> vertices;

    bool aHasMoreVertices = a.GetVertices().size() > b.GetVertices().size();
    CShape outer = aHasMoreVertices ? a : b;
    CShape inner = aHasMoreVertices ? b : a;

    int outerIndex = 0;
    int innerIndex = 0;

    while (true)
    {
        Vertex v1 = outer.GetVertices()[outerIndex];
        Vertex v2 = inner.GetVertices()[innerIndex];

        Vector3 pos = v1.m_Position - v2.m_Position;
        PushVertexIfNotPresent(vertices, pos);

        if (innerIndex < inner.GetVertices().size() - 1)
        {
            innerIndex++;
            v2 = inner.GetVertices()[innerIndex];
            pos = v1.m_Position - v2.m_Position;
            PushVertexIfNotPresent(vertices, pos);
        }

        if (outerIndex < outer.GetVertices().size() - 1)
        {
            outerIndex++;
            v1 = outer.GetVertices()[outerIndex];
            pos = v1.m_Position - v2.m_Position;
            PushVertexIfNotPresent(vertices, pos);
        }

        if (outerIndex >= outer.GetVertices().size() - 1 && innerIndex >= inner.GetVertices().size() - 1)
        {
            break;
        }
    }

    return CShape{ vertices };
}

int main()
{
    // Not using unique pointers purposefully, we know the size of an instance of CShape is X, we can get a tonne of them on the stack if we want.
    CShape triangle({ Vertex{-2.0f, 2.0f, 0.0f}, Vertex{-2.0f, -2.0f, 0.0f}, Vertex{-1.0f, 0.0f, 0.0f} });
    Vertex triangleSupportPoint = triangle.FindMostExtremePoint();
    OutputShapeDebug(triangle, "Triangle");

    CShape square({ Vertex{ 0.0f, 0.0f, 0.0f }, Vertex{ 0.0f, -2.0f, 0.0f }, Vertex{ 2.0f, 0.0f, 0.0f }, Vertex{2.0f, -2.0f, 0.0f} });
    Vertex squareSupportPoint = square.FindOppositeSupportPoint(triangleSupportPoint);
    OutputShapeDebug(square, "Square");

    CShape minkowskiSupportPoints{ {triangleSupportPoint, squareSupportPoint} };
    OutputShapeDebug(minkowskiSupportPoints, "Support Points");

    CShape minkowskiSum = CalculateMinkowskiDifference(triangle, square);
    Vertex mostExtremeVertex = minkowskiSum.FindMostExtremePoint();
    OutputShapeDebug(minkowskiSum, "Minkowski");

    bool originInPoint = false;
    int i = 0;
    for (Vertex v : minkowskiSum.GetVertices())
    {
        if (v == squareSupportPoint || v == triangleSupportPoint)
        {
            continue;
        }

        CShape testTriangle = CShape({ v.m_Position, triangleSupportPoint.m_Position, squareSupportPoint.m_Position });
        bool intersection = IsPointInTriangle(Vector3(0.0f, 0.0f, 0.0f), v, triangleSupportPoint, squareSupportPoint);
        if (intersection)
        {
            cout << "Testing shape " << i << endl;
            OutputShapeDebug(testTriangle, "Testing shape: ");
            cout << "INTERSECT!" << endl;
        }
        originInPoint = originInPoint || intersection;
        i++;
    }

    if (originInPoint)
    {
        cout << "These shapes intersect" << endl;
    }
    else
    {
        cout << "These shapes do NOT intersect" << endl;
    }
}