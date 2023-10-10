#include <iostream>
#include <memory>
#include <vector>

#include "vertex.h"
#include "shape.h"

using namespace std;

constexpr int k_DebugRowCount = 5;
constexpr int k_DebugColCount = 5;

enum class MinkowskiType
{
    Sum,
    Difference
};

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

CShape CalculateMinkowskiShape(const MinkowskiType& minkowskiType, const CShape& a, const CShape& b)
{
    bool aHasMoreVertices = a.GetVertices().size() > b.GetVertices().size();
    std::vector<Vertex> aCyclicVertices =  aHasMoreVertices ? a.GetVertices() : b.GetVertices();
    std::vector<Vertex> bCyclicVertices = aHasMoreVertices ? b.GetVertices() : a.GetVertices();

    aCyclicVertices.push_back(aCyclicVertices[0]);
    aCyclicVertices.push_back(aCyclicVertices[1]);
    bCyclicVertices.push_back(bCyclicVertices[0]);
    bCyclicVertices.push_back(bCyclicVertices[1]);

    std::vector<Vertex> vertices;
    int i = 0;
    int j = 0;

    float sign = 1.0f;
    switch (minkowskiType)
    {
    case MinkowskiType::Difference:
    {
        sign = -1.0f;
        break;
    }
    }

    while (i != aCyclicVertices.size() - 2 && j != bCyclicVertices.size() - 2)
    {
        vertices.push_back(aCyclicVertices[i] + (bCyclicVertices[j] * sign));
        Vector3 crossA = (aCyclicVertices[i + 1] - aCyclicVertices[i]).m_Position;
        Vector3 crossB = (bCyclicVertices[j + 1] - bCyclicVertices[j]).m_Position; 
        Vector3 crossProd = Vector3::CrossProd(crossA, crossB);
        if (crossProd.m_Z >= 0.0f && i < aCyclicVertices.size() - 2)
        {
            i++;
        }
        if (crossProd.m_Z < 0.0f && j < bCyclicVertices.size() - 2)
        {
            j++;
        }
    }

    return CShape{ vertices };
}

int main()
{
    // Not using unique pointers purposefully, we know the size of an instance of CShape is X, we can get a tonne of them on the stack if we want.
    CShape triangle({ Vertex{-4.0f, 2.0f, 0.0f}, Vertex{-4.0f, -2.0f, 0.0f}, Vertex{5.0f, 0.0f, 0.0f} });
    Vertex triangleSupportPoint = triangle.FindMostExtremePoint();
    OutputShapeDebug(triangle, "Triangle");

    CShape square({ Vertex{ 0.0f, 0.0f, 0.0f }, Vertex{ 0.0f, -2.0f, 0.0f }, Vertex{ 2.0f, 0.0f, 0.0f }, Vertex{2.0f, -2.0f, 0.0f} });
    Vertex squareSupportPoint = square.FindOppositeSupportPoint(triangleSupportPoint);
    OutputShapeDebug(square, "Square");

    CShape minkowskiSupportPoints{ {triangleSupportPoint, squareSupportPoint} };
    OutputShapeDebug(minkowskiSupportPoints, "Support Points");

    CShape minkowskiShape = CalculateMinkowskiShape(MinkowskiType::Difference, triangle, square);
    Vertex mostExtremeVertex = minkowskiShape.FindMostExtremePoint();
    OutputShapeDebug(minkowskiShape, "Minkowski");

    bool originInPoint = false;
    int i = 0;
    for (Vertex v : minkowskiShape.GetVertices())
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