#include <cstdlib>
#include <iostream>
#include "Explosion.h"

Explosion::Explosion(const sf::Texture& texture)
: sf::Sprite(texture), exploding(false), cnt(0)
{
    //texture.loadFromFile(filename);
    //explosion.setTexture(texture);
   // explosion.setOrigin(explosion.getLocalBounds().width/2.0f,explosion.getLocalBounds().height / 2.0f);
    setOrigin(sf::Vector2f(getLocalBounds().size.x/2.0f,getLocalBounds().size.y / 2.0f));
}

void Explosion::update()
{
    ++cnt;
    if (cnt > 200)
    {
        exploding = false;
    }
    rotate(sf::degrees(90));
}
