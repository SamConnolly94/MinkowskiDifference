#pragma once

namespace MinkowskiTests
{
    bool RunAllTests();
    
    bool TriangleSquare_NoIntersection();
    bool TriangleSquare_Intersection();
    
    bool PentagonPentagon_Intersection();
    bool PentagonPentagon_NoIntersection();

    bool HexagonTriangle_NoIntersection();
    bool HexagonTriangle_Intersection();
}