#include "Constants.h"
#include "Bullet.h"

Bullet::Bullet(const sf::Vector2f& pos)
    : sf::CircleShape(BulletRadius)
{
    setOrigin(sf::Vector2f(getLocalBounds().size.x / 2.0f, getLocalBounds().size.y / 2.0f));
    setPosition(pos);
    setFillColor(sf::Color::Yellow);
}

bool Bullet::move()
{
    setPosition(sf::Vector2f(getPosition().x, getPosition().y - BulletSpeed));
    if (getPosition().y < 40.0f)
        return false;
    return true;
}

