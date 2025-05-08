#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <string>

#include <SFML/Graphics.hpp>


class Explosion : public sf::Sprite
{
public:
    Explosion(const sf::Texture& texture);
    bool isExploding() const
    {
        return exploding;
    }
    const sf::Sprite& getExplosion() const
    {
        return *this;
    }
    void startExplosion(sf::Vector2f location)
    {
        setPosition(location);
        cnt = 0;
        exploding = true;
    }
    void update();

private:
    bool exploding;
    int cnt;
};

#endif // EXPLOSION_H
