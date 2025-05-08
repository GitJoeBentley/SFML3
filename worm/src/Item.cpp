#include <iostream>
#include <cmath>

#include "Item.h"

Item::Item(const sf::Texture& texture, const std::vector<Item*>& itemVector, float sz)
    : sprite(texture), size(sz,sz), itemsInExistence(itemVector)

{
    sprite.setScale(size);
    sf::Vector2f origin(sprite.getLocalBounds().size.x/2.0f,sprite.getLocalBounds().size.y / 2.0f);
    sprite.setOrigin(origin);
    move();
}

void Item::move()
{
    float xpos = 0.0f, ypos;
    // Make sure item is not close to the snake
    bool newPositionOk = false;

    do
    {
        do
            xpos = rand()%static_cast<int>(BackgroundSize.x-16)+16.0f;
        while (fabs(xpos - BackgroundSize.x/2.0f) < 40.0f);

        ypos = rand()%static_cast<int>(BackgroundSize.y-64.0f)+16.0f;

        newPositionOk = true;

        for (auto it = itemsInExistence.cbegin(); it != itemsInExistence.cend(); ++it)
        {
            if ((fabs((*it)->getSprite().getPosition().x - xpos) < 30.0f) && (fabs((*it)->getSprite().getPosition().y - ypos) < 30.0f))
            {
                newPositionOk = false;
                break;
            }
        }
    }
    while (!newPositionOk);
    sprite.setPosition(sf::Vector2f(xpos,ypos));
    //std::cout << "Item position: " << xpos << ',' << ypos << std::endl;
}
