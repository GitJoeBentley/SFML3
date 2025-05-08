#ifndef METEOR_H
#define METEOR_H

#include <SFML/Graphics.hpp>

extern sf::Vector2f BackgroundSize;
extern sf::Texture StarTexture;

class Meteor
{
public:
    Meteor();
    virtual ~Meteor() = default;
    void move();
    enum Direction {N,NE,E,SE,S,SW,W,NW};
    sf::Sprite get_meteor() const { return meteor;}
    void assignNewPosition();
private:
    sf::Sprite meteor;
    sf::Vector2f position;
    Direction direction;
    void assignDirection();
};

float rand_float(float range);

#endif // METEOR_H
