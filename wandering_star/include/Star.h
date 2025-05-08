#ifndef STAR_H
#define STAR_H

#include <SFML/Graphics.hpp>
#include "StationarySprite.h"

extern sf::Vector2f BackgroundSize;
extern sf::Texture StarTexture;

class Star
{
public:
    enum Direction {N,NE,E,SE,S,SW,W,NW,NONE};
    Star();
    virtual ~Star() = default;
    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const { return position; }
    Direction move(Direction = NONE);
    sf::Sprite getStar() { return star; }
    bool collideWithSprite(const sf::Sprite& sprite);
    Direction getOppositeDirection(Direction);
private:
    sf::Sprite star;
    sf::Vector2f position;
};

#endif // STAR_H
