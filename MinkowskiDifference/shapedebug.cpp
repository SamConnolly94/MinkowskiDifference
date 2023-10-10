#include "shapedebug.h"

#include "shape.h"
#include "vertex.h"

#include <algorithm>

// Wouldn't normally use namespace::std, but trying to keep this clean :)
using namespace std;

std::string CShapeDebug::GetDebugGrid(const CShape& shape, string title)
{
    std::string debugStr = "";

    debugStr += "\r\n===========================\r\n" + title + "\r\n===========================\r\n";

    // Output coords above
    for (int x = -k_DebugColCount; x <= k_DebugColCount; x++)
    {
        if (x >= 0)
        {
            debugStr += " " + std::to_string(x);
        }
        else
        {
            debugStr += std::to_string(x);
        }
    }
    debugStr += "\r\n";

    for (int x = -k_DebugColCount; x < k_DebugColCount; x++)
    {
        debugStr += " _";
    }
    debugStr += "\r\n";

    for (int y = -k_DebugRowCount; y <= k_DebugRowCount; y++)
    {
        for (int x = -k_DebugColCount; x < k_DebugColCount; x++)
        {
            auto it = find_if(shape.GetVertices().begin(), shape.GetVertices().end(), [&](const Vertex& vertex) {
                return static_cast<float>(x) == vertex.m_Position.m_X && static_cast<float>(y) == vertex.m_Position.m_Y;
                });

            if (it != shape.GetVertices().end())
            {
                debugStr += "|*";
            }
            else
            {
                debugStr += "|_";
            }
        }

        debugStr += "| " + std::to_string(y) + "\r\n";
    }
    debugStr += "\r\n";
    return debugStr;
}
