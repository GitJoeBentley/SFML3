#ifndef GUN_H
#define GUN_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Invaders.h"
#include "Bullet.h"
#include "Bomb.h"

class Gun : public sf::Sprite
{
public:
    enum Direction {Right, Left};
    static sf::Vector2f GunStartPosition;
    Gun(sf::Texture& texture);
    const sf::Sprite& getGun() const
    {
        return *this;
    }
    void move(Direction dir);
    void moveToStartPosition();
    sf::Vector2f size() const
    {
        return sf::Vector2f(getLocalBounds().size.x * getScale().x, getLocalBounds().size.y * getScale().y);
    }
    Bullet* shoot() const;
    bool hitByBomb(Bomb& bomb);
    bool isVisible() const { return visible; }
    void setInvisible() { visible = false; }
    void setVisible() { visible = true; }
    void moveToPosition(sf::Vector2f pos = sf::Vector2f(GunStartPosition));

private:
    bool visible;
};

#endif // GUN_H
