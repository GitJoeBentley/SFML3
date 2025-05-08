#ifndef SHIELD_H
#define SHIELD_H

#include <stdint.h>
#include <SFML/Graphics.hpp>

#include "Constants.h"
#include "ShieldImage.h"
#include "Invaders.h"
#include "Bomb.h"

class Shield
{
public:
    Shield();
    ~Shield();

    void getShieldDataFromFile(uint64_t* InitialShieldData);

    const sf::Sprite& getShield() const
    {
        return *shield;
    }
    float topOfShield() const
    {
        return shield->getPosition().y - ShieldSize.y / 2.0f;
    }
    float bottomOfShield() const
    {
        return shield->getPosition().y + ShieldSize.y / 2.0f;
    }
    float leftSidePosition() const
    {
        return shield->getPosition().x - 32.0f;
    }
   float rightSidePosition() const
    {
        return shield->getPosition().x + 32.0f;
    }
    bool isAlignedWithBomb(const Bomb& bomb) const;
    bool isAlignedWithBullet(const Bullet& bullet) const;
    bool hitByBomb(sf::Vector2f shieldPosition);
    bool hitByBullet(sf::Vector2f shieldPosition);

    //static sf::Image createShieldImage();

private:
    uint64_t* shieldData;
    sf::Image* shieldImage;
    sf::Texture* shieldTexture;
    sf::Sprite* shield;
};

#endif // SHIELD_H
