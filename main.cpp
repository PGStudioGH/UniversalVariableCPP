#include <iostream>

#include "UniversalVariable.h"

int main()
{
    var a = { 5, " from ", { ". So very good!", 1998, 5 }, "I love C++" };

    std::cout << a[3] << a[1] << a[2][1] << a[2][0];

    return 0;
}
