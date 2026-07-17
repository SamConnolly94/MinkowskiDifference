#include "tests.h"
#include <iostream>

int main()
{
    if (!MinkowskiTests::RunAllTests())
    {
        return 1;
    }

    std::cin.get();

    return 0;
}