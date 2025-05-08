#include <iostream>
#include "Meteor.h"

Meteor::Meteor() : meteor(StarTexture)
{
    float scale = 0.01f;
    meteor.setScale(sf::Vector2f(scale,scale));
}

void Meteor::move()
{
    switch (direction)
    {
    case N:
        position.y += -1.0;
        break;
    case S:
        position.y += 1.0;
        break;
    case E:
        position.x += 1.0;
        break;
    case W:
        position.x += -1.0;
        break;
    case NE:
        position.x += 1.0;
        position.y += -1.0;
	break;
    case NW:
        position.x += -1.0;
        position.y += -1.0;
        break;
    case SE:
        position.x += 1.0;
        position.y += 1.0;
        break;
    case SW:
        position.x += -1.0;
        position.y += 1.0;
        break;
    default:
        ;
    }
    meteor.setPosition(sf::Vector2f(position.x,position.y));
    //  std::cout << position.x << ' ' << position.y << "  " << static_cast<int>(direction) << std::endl;
}

void Meteor::assignNewPosition()
{
    position = sf::Vector2f(rand_float(BackgroundSize.x), rand_float(0.9f * BackgroundSize.y));
    meteor.setPosition(position);
    assignDirection();
    //  std::cout << "New position/direction: " << position.x << ' ' << position.y << "  " << static_cast<int>(direction) << std::endl;
}


void Meteor::assignDirection()
{
    sf::Vector2f& pos = position;

    if (pos.x < 0.33f*BackgroundSize.x)
    {
        if (pos.y < 0.33f * BackgroundSize.y)
            direction =  SE;
        else if (pos.y > 0.67f * BackgroundSize.y)
            direction =  NE;
        else
            direction =  E;
    }
    else if (pos.x > 0.67f*BackgroundSize.x)
    {
        if (pos.y < 0.33f * BackgroundSize.y)
            direction =  SW;
        else if (pos.y > 0.67f * BackgroundSize.y)
            direction =  NW;
        else
            direction =  W;
    }
    else
    {
        if (pos.y < 0.5f * BackgroundSize.y)
            direction =  S;
        else
            direction =  N;
    }
}

float rand_float(float range)
{
    return static_cast<float>((rand() % static_cast<int>(range)));
}
