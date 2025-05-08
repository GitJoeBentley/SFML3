#ifndef SAUCER_H
#define SAUCER_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#include "Bullet.h"

class Saucer : public sf::Sprite
{
public:
    enum Direction {East, West};
    Saucer(const sf::Texture& texture);
    sf::Vector2f size() const
    {
        return sf::Vector2f(getLocalBounds().size.x * getScale().x, getLocalBounds().size.y * getScale().y);
    }
    bool move();
    bool hitByBullet(const Bullet& bullet) const;

private:
    Direction direction;
};

#endif // SAUCER_H
