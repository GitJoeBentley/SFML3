#ifndef STATIONARYSPRITE_H
#define STATIONARYSPRITE_H

#include <SFML/Graphics.hpp>
#include "Star.h"
#include "StationarySprite.h"

extern sf::Vector2f BackgroundSize;

class StationarySprite
{
public:
    enum Direction {Up,Down,Left,Right};
    StationarySprite(const sf::Texture&, float ypos);
    virtual ~StationarySprite() = default;
    sf::Vector2f getPosition() const
    {
        return position;
    }
    sf::Sprite getSprite() const { return sprite; }
    void move(Direction);
private:
    sf::Sprite sprite;
    sf::Vector2f position;
};

#endif // STATIONARYSPRITE_H

