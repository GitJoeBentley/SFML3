#include <iostream>
#include <cmath>
#include "Worm.h"

Worm::Worm(const sf::Texture& headTexture, const sf::Texture& texture)
    : head(headTexture), segmentTexture(texture)
{
    setDirection(Up);
    sf::Vector2f origin(head.getLocalBounds().size.x/2.0f,head.getLocalBounds().size.y / 2.0f);
    head.setOrigin(origin);
    head.setPosition(sf::Vector2f(BackgroundSize.x/2.0f,BackgroundSize.y/2.0f));
    segments.push_back(sf::Sprite(segmentTexture));
    segments.push_back(sf::Sprite(segmentTexture));
    segments.push_back(sf::Sprite(segmentTexture));

    float i = 16.0f;
    sf::Vector2f pos;
    for (auto it=segments.begin(); it != segments.end(); ++it, i += 16.0f)
    {
        pos = sf::Vector2f(head.getPosition().x,head.getPosition().y + i);
        it->setPosition(pos);
        origin = sf::Vector2f(it->getLocalBounds().size.x/2.0f,it->getLocalBounds().size.y / 2.0f);
        it->setOrigin(origin);
    }
}

Worm::~Worm()
{
    //dtor
}

void Worm::draw(sf::RenderWindow& window)
{
    window.draw(head);
    for (auto it=segments.begin(); it != segments.end(); ++it)
        window.draw(*it);
}

void Worm::move()
{
    sf::Vector2f pos;
    for (auto i = segments.size()-1; i > 0; --i)
    {
        pos = sf::Vector2f(segments[i-1].getPosition().x, segments[i-1].getPosition().y);
        segments[i].setPosition(pos);
    }
    pos = sf::Vector2f(head.getPosition().x,head.getPosition().y);
    segments[0].setPosition(pos);
    if (direction == Up)
    {
        pos = sf::Vector2f(head.getPosition().x,head.getPosition().y - 16.0f);
        head.setPosition(pos);
    }
    if (direction == Down)
    {
        pos = sf::Vector2f(head.getPosition().x,head.getPosition().y + 16.0f);
        head.setPosition(pos);
    }
    if (direction == Right)
    {
        pos = sf::Vector2f(head.getPosition().x + 16.0f,head.getPosition().y);
        head.setPosition(pos);
    }
    if (direction == Left)
    {
        pos = sf::Vector2f(head.getPosition().x - 16.0f,head.getPosition().y);
        head.setPosition(pos);
    }
}

void Worm::moveOppositeDirection()
{
    switch (direction)
    {
    case Up:
        direction = Down;
        break;
    case Down:
        direction = Up;
        break;
    case Left:
        direction = Right;
        break;
    case Right:
        direction = Left;
        break;
    default:
        ;
    }
    setDirection(direction);
    move();
}

void Worm::setDirection(Direction direction)
{
    this->direction = direction;
    static float currentRotationAngle = 0.0f;
    sf::Angle a;
    if (direction == Up)
    {
        if (currentRotationAngle == 0.0f)
        {
            head.rotate(sf::degrees(0));
        }
        if (currentRotationAngle == 90.0f)
        {
            head.rotate(sf::degrees(-90));
        }
        if (currentRotationAngle == 180.0f)
        {
            head.rotate(sf::degrees(180));
        }
        if (currentRotationAngle == -90.0f)
        {
            head.rotate(sf::degrees(90));
        }
    }
    if (direction == Down)
    {
        if (currentRotationAngle == 0.0f)
            head.rotate(sf::degrees(180));
        if (currentRotationAngle == 90.0f)
            head.rotate(sf::degrees(90));
        if (currentRotationAngle == 180.0f)
            head.rotate(sf::degrees(0));
        if (currentRotationAngle == -90.0f)
            head.rotate(sf::degrees(-90));
        currentRotationAngle = 180.0f;
    }
    if (direction == Left)
    {
        if (currentRotationAngle == 0.0f)
            head.rotate(sf::degrees(-90));
        if (currentRotationAngle == 90.0f)
            head.rotate(sf::degrees(180));
        if (currentRotationAngle == 180.0f)
            head.rotate(sf::degrees(90));
        if (currentRotationAngle == -90.0f)
            head.rotate(sf::degrees(0));
        currentRotationAngle = -90.0f;
    }
    if (direction == Right)
    {
        if (currentRotationAngle == 0.0f)
            head.rotate(sf::degrees(90));
        if (currentRotationAngle == 90.0f)
            head.rotate(sf::degrees(0));
        if (currentRotationAngle == 180.0f)
            head.rotate(sf::degrees(-90));
        if (currentRotationAngle == -90.0f)
            head.rotate(sf::degrees(180));
        currentRotationAngle = 90.0f;
    }
}

bool Worm::hitTheWall()
{
    if ((head.getPosition().x <= 16.0f/2 || head.getPosition().x >= BackgroundSize.x) ||
            (head.getPosition().y <= 16.0f/2 || head.getPosition().y >= BackgroundSize.y))
    {
        //  std::cout << "You hit the wall at " << head.getPosition().x << ',' << head.getPosition().y << std::endl;
        return true;
    }
    return false;
}

bool Worm::collideWith(const sf::Sprite& sprite)
{
    float width = head.getLocalBounds().size.x * head.getScale().x;
    float height = head.getLocalBounds().size.y * head.getScale().y;
    float spriteWidth = sprite.getLocalBounds().size.x * sprite.getScale().x;
    float spriteHeight = sprite.getLocalBounds().size.y * sprite.getScale().y;
    float sumOfHalfWidths = width/2.0f + spriteWidth/2.0f;
    float sumOfHalfHeights = height/2.0f + spriteHeight/2.0f;
    sf::Vector2f spritePos = sprite.getPosition();

    // Collision tolerance tweak
    sumOfHalfHeights *= 0.87f;
    sumOfHalfWidths *= 0.87f;

    if (((fabs(head.getPosition().x-spritePos.x) <= sumOfHalfWidths)) && ((fabs(head.getPosition().y-spritePos.y) <= sumOfHalfHeights)))
    {
        //  std::cout << "Collision at " << sprite.getPosition().x << ',' << sprite.getPosition().x << std::endl;
        return true;
    }
    return false;
}

void Worm::addSegment()
{
    auto numSegments = segments.size();
    auto locationOfLastSegment = segments[numSegments-1].getPosition();
    auto locationOfNextToLastSegment = segments[numSegments-2].getPosition();
    Direction directionOfTail;
    float newXpos, newYpos;

    // std::cout << "Number of segments = " << numSegments << std::endl;
    //  std::cout << "Location of last segment " << locationOfLastSegment.x << ',' << locationOfLastSegment.y << std::endl;
    //  std::cout << "Location of next-to-last segment " << locationOfNextToLastSegment.x << ',' << locationOfNextToLastSegment.y << std::endl;
    // traveling up or down?
    if (locationOfLastSegment.x == locationOfNextToLastSegment.x)
    {
        if (locationOfLastSegment.y > locationOfNextToLastSegment.y)
            directionOfTail = Up;
        else
            directionOfTail = Down;
    }
    else
    {
        if (locationOfLastSegment.x > locationOfNextToLastSegment.x)
            directionOfTail = Left;
        else
            directionOfTail = Right;
    }
    //std::cout << "Direction of tail travel = " << directionOfTail << std::endl;
    segments.push_back(sf::Sprite(segmentTexture));

    switch (directionOfTail)
    {
    case Up:
        newXpos = locationOfLastSegment.x;
        newYpos = locationOfLastSegment.y + 16.0f;
        break;
    case Down:
        newXpos = locationOfLastSegment.x;
        newYpos = locationOfLastSegment.y - 16.0f;
        break;
    case Left:
        newXpos = locationOfLastSegment.x - 16.0f;
        newYpos = locationOfLastSegment.y;
        break;
    case Right:
        newXpos = locationOfLastSegment.x + 16.0f;
        newYpos = locationOfLastSegment.y;
        break;
    default:
        ;
    }

    segments[segments.size()-1].setPosition(sf::Vector2f(newXpos,newYpos));
    segments[segments.size()-1].setOrigin(sf::Vector2f(segments[segments.size()-1].getLocalBounds().size.x/2.0f,segments[segments.size()-1].getLocalBounds().size.y / 2.0f));
}

