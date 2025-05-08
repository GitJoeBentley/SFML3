#include "Star.h"

#include <iostream>

StationarySprite::StationarySprite(const sf::Texture& texture, float ypos)
    : sprite(texture),
      position(50.0f+.8f*(rand() % static_cast<int>(BackgroundSize.x)),
               static_cast<float>( rand() % static_cast<int>(BackgroundSize.y * ypos + 50.0f)))
{
    sprite.setScale(sf::Vector2f(0.3f,0.3f));
    sf::FloatRect spriteRect = sprite.getLocalBounds();
    float spriteXpos = spriteRect.size.x/2.0f;
    float spriteYpos = spriteRect.size.y/2.0f;
    sprite.setOrigin(sf::Vector2f(spriteXpos, spriteYpos));
    sprite.setPosition(position);
    sf::Angle rot = sf::degrees(90.0f * (position.x/BackgroundSize.x-0.33f));
    sprite.rotate(rot);
}

void StationarySprite::move(Direction direction)
{
    // Move sprite 5% of window size
    sf::Vector2f pos;
    switch (direction)
    {
    case Up:
        pos = sf::Vector2f(sprite.getPosition().x, sprite.getPosition().y - 0.05f * BackgroundSize.y);
        sprite.setPosition(pos);
        if (sprite.getPosition().y < 0.0f)
        {
            pos = sf::Vector2f(sprite.getPosition().x,BackgroundSize.y - 1.0f);
            sprite.setPosition(pos);
        }
        break;
    case Down:
        pos = sf::Vector2f(sprite.getPosition().x, sprite.getPosition().y + 0.05f * BackgroundSize.y);
        sprite.setPosition(pos);
        if (sprite.getPosition().y > BackgroundSize.y)
        {
            pos = sf::Vector2f(sprite.getPosition().x, 1.0f);
            sprite.setPosition(pos);
        }
        break;
    case Left:
        pos = sf::Vector2f(sprite.getPosition().x - 0.05f * BackgroundSize.x,sprite.getPosition().y);
        sprite.setPosition(pos);
        if (sprite.getPosition().x < 0.0f)
        {
            pos = sf::Vector2f(BackgroundSize.x - 1.0f,sprite.getPosition().y);
            sprite.setPosition(pos);
        }
        break;
    case Right:
        pos = sf::Vector2f(sprite.getPosition().x + 0.05f * BackgroundSize.x,sprite.getPosition().y);
        sprite.setPosition(pos);
        if (sprite.getPosition().x > BackgroundSize.x)
        {
            pos = sf::Vector2f(1.0f,sprite.getPosition().y);
            sprite.setPosition(pos);
        }
        break;
    default:
        ;
    };
}
