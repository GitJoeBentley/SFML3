#ifndef NUMBERS_H
#define NUMBERS_H
#include <vector>
#include <SFML/Graphics.hpp>
//#include "TextBox.h"
#include "NumberCircle.h"
#include "Problem.h"

class Numbers
{
public:
    Numbers(sf::RenderWindow& renderWindow, const Problem& problem_, const TextBox& problemTB_);
    ~Numbers();
    void draw();
    bool circleIntersectsAnotherCircle(const NumberCircle* circle);
    NumberCircle* numberCircle(size_t index) { return numbers[index]; }
    const sf::CircleShape& getCircle(size_t index)  const { return numbers[index]->getCircle(); }
    sf::Vector2f getCirclePosition(size_t index)  const { return numbers[index]->getCircle().getPosition(); }
    void removeCircle(size_t index) { delete numbers[index]; numbers[index] = nullptr; }
    int getIndex(int value);

private:
    sf::RenderWindow& window;
    const Problem& problem;
    const TextBox& problemTB;
    std::vector<NumberCircle*> numbers;

    sf::Vector2f getGoodPosition() const;
};

#endif // NUMBERS_H
