#include "Problem.h"
#include <cstdlib>
using namespace std;

Problem::Problem(int typ, int level)
    : type(typ), number1(rand() % (level * 10)), number2(rand() % (level * 10))
{
    if (typ == 2) type = rand() % 2;
    else if (typ == 3) type = 2;
    else if (typ == 4) type = rand() % 3;

    if ((type == 1) && number1 < number2) {
        swap(number1, number2);
    }
}

Problem::operator std::string() const
{
    return to_string(number1) + " "
           + ("+-x"[static_cast<int>(type)])
           + " " + to_string(number2);
}

Problem::operator int() const
{
    switch (type)
    {
    case 0:
        return number1 + number2;
    case 1:
        return number1 - number2;
    case 2:
        return number1 * number2;
    default:
        ;
    }
    return -1;
}
