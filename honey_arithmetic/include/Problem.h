#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>
#include "TextBox.h"

class Problem
{
public:
    //enum class Type : int {Add, Subtract, Multiply, Divide};
    Problem(int type, int level = 1);
    operator std::string() const;
    operator int() const;

private:
    int type;
    int number1;
    int number2;

};

#endif // PROBLEM_H
