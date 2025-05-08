#include <iostream>
#include <cmath>
#include <SFML/Graphics/Rect.hpp>

#include "Star.h"

Star::Star() : star(StarTexture)
{
    star.setScale(sf::Vector2f(0.03f,0.03f));
    position = sf::Vector2f(rand() % static_cast<int>(BackgroundSize.x), rand() % static_cast<int>(0.89f*BackgroundSize.y));
}

Star::Direction Star::move(Direction direction)
{
    static int LastMove = 0;
    int tempRand = 0;
    if (direction == NONE)
    {
        tempRand = rand()%100;
        tempRand = tempRand > 7 ? LastMove:tempRand;
        direction = tempRand < 8 ? static_cast<Direction>(tempRand) : NONE;
    }
    switch (direction)
    {
    case N:
        position.y -= 1.0f;
        break;
    case S:
        position.y += 1.0f;
        break;
    case E:
        position.x += 1.0f;
        break;
    case W:
        position.x -= 1.0f;
        break;
    case NE:
        position.x += 1.0f;
        position.y -= 1.0f;
        break;
    case SE:
        position.x += 1.0f;
        position.y += 1.0f;
        break;
    case NW:
        position.x -= 1.0f;
        position.y -= 1.0f;
        break;
    case SW:
        position.x -= 1.0f;
        position.y += 1.0f;
        break;
    default: // don't move
        ;
    }

    LastMove = tempRand;

    if (position.x >= BackgroundSize.x-1.0f)
        position.x = 2.0f;
    if (position.x < 2.0)
        position.x = BackgroundSize.x-2.0f;
    if (position.y < 2.0)
        position.y = 3.0f;
    if (position.y > 0.85f*BackgroundSize.y)
        position.y = 0.85f*BackgroundSize.y;

    star.setPosition(position);
    return direction;
//   std::cout << "Star location: " << position.x << ' ' << position.y << std::endl;
}

void Star::setPosition(const sf::Vector2f& pos)
{
    position.x = pos.x;
    position.y = pos.y;
}

bool Star::collideWithSprite(const sf::Sprite& sprite)
{
    sf::FloatRect starLB = star.getLocalBounds();
    sf::Vector2f starSize = starLB.size;
    sf::FloatRect spriteLB = sprite.getLocalBounds();
    sf::Vector2f spriteSize = spriteLB.size;
    float width = starSize.x * star.getScale().x;
    float height = starSize.y * star.getScale().y;
    float spriteWidth = spriteSize.x * sprite.getScale().x;
    float spriteHeight = spriteSize.y * sprite.getScale().y;
    float sumOfHalfWidths = width/2.0f + spriteWidth/2.0f;
    float sumOfHalfHeights = height/2.0f + spriteHeight/2.0f;
    sf::Vector2f spritePos = sprite.getPosition();
    if (((fabs(position.x-spritePos.x) <= sumOfHalfWidths)) && ((fabs(position.y-spritePos.y) <= sumOfHalfHeights)))
    {
        std::cout << "\a";
        return true;
    }
    return false;
}

Star::Direction Star::getOppositeDirection(Direction direction)
{
    switch (direction)
    {
    case N:
        return S;
    case S:
        return N;
    case E:
        return W;
    case W:
        return E;
    case NE:
        return SW;
    case NW:
        return SE;
    case SE:
        return NW;
    case SW:
        return NE;
    default:
        ;
    }
    return NONE;
}
