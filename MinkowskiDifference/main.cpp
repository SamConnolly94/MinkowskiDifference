#include "tests.h"

int main()
{
    if (!MinkowskiTests::RunAllTests())
    {
        return 1;
    }

    return 0;
}