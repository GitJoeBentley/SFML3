#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Constants.h"
#include "Saucer.h"
#include "Invaders.h"

Saucer::Saucer(const sf::Texture& texture)
    : sf::Sprite(texture), direction(static_cast<Direction>(rand()%2))
{
    setScale(sf::Vector2f(1.0f,1.0f));
    setOrigin(sf::Vector2f(getLocalBounds().size.x/2.0f,getLocalBounds().size.y / 2.0f));
    setPosition((direction == East) ? sf::Vector2f(0.0f,80.0f): sf::Vector2f(static_cast<float>(MainWindowWidth),80.0f));
}

// Return true if the saucer is still inside the main window
bool Saucer::move()
{
    if (direction == East)
    {
        setPosition(sf::Vector2f(getPosition().x + SaucerSpeed, getPosition().y));
        if (getPosition().x < static_cast<float>(MainWindowWidth))
            return true;
    }
    else
    {
        setPosition(sf::Vector2f(getPosition().x - SaucerSpeed, getPosition().y));
        if (getPosition().x > 0.0f)
            return true;
    }
    return false;
}

bool Saucer::hitByBullet(const Bullet& bullet) const
{
    bool hit;
    // if bullet is below Saucer, there is no hit
    if ((getPosition().y - bullet.getPosition().y) > size().y / 2.0f)
        return false;

    float diffX = fabs(getPosition().x - bullet.getPosition().x);
    float diffY = fabs(getPosition().y - bullet.getPosition().y);
    hit = std::sqrt(diffX * diffX + diffY * diffY) < size().x / 2.0f;
    return hit;
}

