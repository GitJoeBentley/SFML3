#include "Numbers.h"
#include "constants.h"
#include "proto.h"
#include <string>
#include <cmath>
using namespace std;

Numbers::Numbers(sf::RenderWindow& renderWindow, const Problem& problem_, const TextBox& problemTB_)
    : window(renderWindow), problem(problem_), problemTB(problemTB_)
{
    NumberCircle* ptrNumberCircle;
    size_t numberCount = 0;
    sf::Vector2f pos;
    std::string numStr;
    sf::FloatRect rect;
    int num = static_cast<int>(problem) - 10;
    if (num < 0) num = 0;

    while (numberCount < NumNumbers)
    {
        pos = getGoodPosition();
        numStr = to_string(num);
        ptrNumberCircle = new NumberCircle(window, numStr, pos, sf::Color::Yellow, sf::Color::Red);
        if (circleIntersectsAnotherCircle(ptrNumberCircle))
        {
            delete ptrNumberCircle;
            ptrNumberCircle = nullptr;
            continue;
        }
        numbers.push_back(ptrNumberCircle);
        num++;
        numberCount++;
    }
}

int Numbers::getIndex(int value)
{
    for (unsigned i = 0; i < numbers.size(); i++)
    {
        if (*(numbers[i]) == value) return i;
    }
    return -1;
}


sf::Vector2f Numbers::getGoodPosition() const
{
    bool isGoodPosition = false;
    sf::Vector2f pos;

    while (!isGoodPosition)
    {
        pos = sf::Vector2f(rand() % 100 / 100.f * window.getSize().x, rand() % 100 / 100.f * window.getSize().y);
        if (pos.y < 0.1 * window.getSize().y) continue;
        if (pos.y > 0.80 * window.getSize().y) continue;
        if (pos.x < 0.03 * window.getSize().x) continue;
        if (pos.x > 0.97  * window.getSize().x) continue;
        if (distanceBetweenTwoPositions(problemTB.getBox().getPosition(),pos) < 0.68f * problemTB.getBox().getSize().x) continue;

        isGoodPosition = true;
    }
    return pos;
}

bool Numbers::circleIntersectsAnotherCircle(const NumberCircle* circle)
{
    for (auto other : numbers)
    {
        if (distanceBetweenTwoPositions(circle->getCircle().getPosition(), other->getCircle().getPosition()) < 2.0f * CircleRadius) return true;
    }
    return false;
}

Numbers::~Numbers()
{
    for (size_t i = 0; i < NumNumbers; i++)
    {
        if (numbers[i]) delete numbers[i];
        numbers[i] = nullptr;
    }
}

void Numbers::draw()
{
    for (size_t i = 0; i < NumNumbers; i++)
    {
        if (numbers[i]) numbers[i]->draw();
    }
}


