#include <cmath>
#include <iostream>
#include "Paddle.h"
#include "constants.h"
using namespace std;

Paddle::Paddle(float width)
    : sf::RectangleShape(sf::Vector2f(width, 16.0f)), direction(Stopped)
{
    sf::FloatRect rect = getLocalBounds();
    setOrigin(sf::Vector2f(rect.position.x + rect.size.x / 2.0f, rect.position.y + rect.size.y / 2.0f));
    moveToStartPosition();
}

float Paddle::getWidth() const
{
    return getSize().x;
}
float Paddle::left() const
{
    sf::FloatRect rect = getGlobalBounds();
    return rect.position.x;
}

float Paddle::right() const
{
    sf::FloatRect rect = getGlobalBounds();
    return rect.position.x + rect.size.x;
}

float Paddle::top() const
{
    return getPosition().y - getSize().y;
}

void Paddle::setWidth(float width)
{
    setSize(sf::Vector2f(width, getSize().y));
}

void Paddle::moveToStartPosition()
{
    setPosition(PaddleStartPosition);
}

void Paddle::move(Direction dir)
{
    direction = dir;
}

void Paddle::move(Direction dir, float distance)
{
    sf::Vector2f newPosition(getPosition());
    if (dir == Right)
    {
        newPosition.x += distance;
    }
    if (dir == Left)
    {
        newPosition.x -= distance;
    }
    setPosition(newPosition);
}

void Paddle::stop()
{
    direction = Stopped;
}

void Paddle::update(sf::Time dt)
{
    sf::Vector2f newPosition(getPosition());
    if (direction == Left)
    {
        newPosition.x -= PaddleSpeed * dt.asSeconds();
    }
    if (direction == Right)
    {
        newPosition.x += PaddleSpeed * dt.asSeconds();
    }
    setPosition(newPosition);
}
