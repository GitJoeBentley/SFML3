#include <cmath>
#include "Gun.h"

sf::Vector2f Gun::GunStartPosition(MainWindowWidth / 2.0f, 0.97f * MainWindowHeight);

Gun::Gun(sf::Texture& texture)
    : sf::Sprite(texture), visible(true)
{
    setPosition(sf::Vector2f(MainWindowWidth / 2.0f, 0.96f * MainWindowHeight));
    setOrigin(sf::Vector2f(getLocalBounds().size.x/2.0f,getLocalBounds().size.y / 2.0f));
}

void Gun::moveToPosition(sf::Vector2f pos)
{
    setPosition(pos);
}

void Gun::move(Direction dir)
{
    if (dir == Right)
    {
        // if the gun is on the right edge, disallow move
        if (getPosition().x < MainWindowWidth - size().x)
            setPosition(sf::Vector2f(getPosition().x + GunSpeed, getPosition().y));
    }
    if (dir == Left)
    {
        // if the gun is on the left edge, disallow move
        if (getPosition().x > size().x)
            setPosition(sf::Vector2f(getPosition().x - GunSpeed, getPosition().y));
    }
}

Bullet* Gun::shoot() const
{
    // Get gun location
    sf::Vector2f gunBarrelEnd = sf::Vector2f(getPosition().x, getPosition().y - (size().y / 1.5f));
    return new Bullet(gunBarrelEnd);
}

bool Gun::hitByBomb(Bomb& bomb)
{
    bool hit;
    sf::Vector2f gunPos = getPosition();
    sf::Vector2f bombPos = bomb.getPosition();
    if (gunPos.y - bombPos.y > (size().y + BombSize.y) / 2.0f) return false;
    float diffX = fabs(gunPos.x - bombPos.x);
    float diffY = fabs(gunPos.y - bombPos.y);
    hit = std::sqrt(diffX * diffX + diffY * diffY) < (size().x + BombSize.x) / 2.0f;
    if (hit) visible = false;
    return hit;
}

