#include "tests.h"

#include "shape.h"
#include "vertex.h"

#include <iostream>

bool MinkowskiTests::RunAllTests()
{
    bool result = true;

    //if (!TriangleSquare_NoIntersection())
    //{
    //    std::cout << "TriangleSquare_NoIntersection test case FAILED." << std::endl;
    //    result = false;
    //}

    if (!TriangleSquare_Intersection())
    {
        std::cout << "TriangleSquare_Intersection test case FAILED." << std::endl;
        result = false;
    }

    if (!PentagonPentagon_NoIntersection())
    {
        std::cout << "PentagonPentagon_Intersection test case FAILED." << std::endl;
        result = false;
    }

    if (!PentagonPentagon_Intersection())
    {
        std::cout << "PentagonPentagon_Intersection test case FAILED." << std::endl;
        result = false;
    }

    return result;
}

bool MinkowskiTests::TriangleSquare_NoIntersection()
{
    std::cout << "===========================================" << std::endl;
    std::cout << "No TriangleSquare_Intersection Tests" << std::endl;
    std::cout << "===========================================" << std::endl;

    CShape triangle("Triangle", { Vertex{-2, 1.0f, 0.0f}, Vertex{-2.0f, -1.0f, 0.0f}, Vertex{-1.0f, 0.0f, 0.0f} });
    CShape square("Square", { Vertex{0.0f, 0.0f, 0.0f}, Vertex{0.0f, -2.0f, 0.0f}, Vertex{2.0f, 0.0f, 0.0f}, Vertex{2.0f, -2.0f, 0.0f} });
    std::cout << triangle << std::endl << square << std::endl;

    return !triangle.IntersectsWith(square);
}

bool MinkowskiTests::TriangleSquare_Intersection()
{
    std::cout << "===========================================" << std::endl;
    std::cout << "TriangleSquare_Intersection Tests" << std::endl;
    std::cout << "===========================================" << std::endl;

    CShape triangle("Triangle", { Vertex{-2, 1.0f, 0.0f}, Vertex{-2.0f, -1.0f, 0.0f}, Vertex{1.0f, 0.0f, 0.0f} });
    CShape square("Square", { Vertex{0.0f, 0.0f, 0.0f}, Vertex{0.0f, -2.0f, 0.0f}, Vertex{2.0f, 0.0f, 0.0f}, Vertex{2.0f, -2.0f, 0.0f} });
    std::cout << triangle << std::endl << square << std::endl;

    return triangle.IntersectsWith(square);
}

bool MinkowskiTests::PentagonPentagon_NoIntersection()
{
    std::cout << "===========================================" << std::endl;
    std::cout << "PentagonPentagon_NoIntersection Tests" << std::endl;
    std::cout << "===========================================" << std::endl;

    CShape p1("Pentagon 1", { Vertex{-3.0f, -4.0f, 0.0f}, Vertex{-4.0f, -3.0f, 0.0f}, Vertex{-2.0f, -2, 0.0f}, Vertex{-1.0f, -3.0f, 0.0f}, Vertex{-2.0f, -4.0f, 0.0f} });
    CShape p2("Pentagon 2", { Vertex{3.0f, -4.0f, 0.0f}, Vertex{4.0f, -3.0f, 0.0f}, Vertex{2.0f, -2, 0.0f}, Vertex{1.0f, -3.0f, 0.0f}, Vertex{1.0f, -4.0f, 0.0f} });
    std::cout << p1 << std::endl << p2 << std::endl;

    return !p1.IntersectsWith(p2);
}


bool MinkowskiTests::PentagonPentagon_Intersection()
{
    std::cout << "===========================================" << std::endl;
    std::cout << "PentagonPentagon_Intersection Tests" << std::endl;
    std::cout << "===========================================" << std::endl;

    CShape p1("Pentagon 1", { Vertex{-3.0f, -4.0f, 0.0f}, Vertex{-4.0f, -2.0f, 0.0f}, Vertex{-2.0f, -1.0f, 0.0f}, Vertex{-1.0f, -2.0f, 0.0f}, Vertex{-2.0f, -4.0f, 0.0f} });
    CShape p2("Pentagon 2", { Vertex{-4.0f, -4.0f, 0.0f}, Vertex{-5.0f, -2.0f, 0.0f}, Vertex{-3.0f, 2.0f, 0.0f}, Vertex{0.0f, -2.0f, 0.0f}, Vertex{-1.0f, -4.0f, 0.0f} });
    std::cout << p1 << std::endl << p2 << std::endl;

    return p1.IntersectsWith(p2);
}