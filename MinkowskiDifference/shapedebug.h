#pragma once

#include <string>

class CShape;

class CShapeDebug
{
private:
    static constexpr int k_DebugRowCount = 8;
    static constexpr int k_DebugColCount = 8;
public:
    static std::string GetDebugGrid(const CShape& shape, std::string title);
};

