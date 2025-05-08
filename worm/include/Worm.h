#ifndef WORM_H
#define WORM_H

#include <vector>

#include <SFML/Graphics.hpp>

extern sf::Vector2f BackgroundSize;

class Worm
{
public:
    Worm(const sf::Texture& headTexture, const sf::Texture& segmentTexture);
    virtual ~Worm();
    void draw(sf::RenderWindow& window);
    enum Direction {Up,Down,Left,Right};
    void move();
    void moveOppositeDirection();
    void setDirection(Direction direction);
    bool hitTheWall();
    bool collideWith(const sf::Sprite& sprite);
    void addSegment();

protected:

private:
    sf::Sprite head;
    Direction direction;
    std::vector<sf::Sprite> segments;
    const sf::Texture& segmentTexture;
};

#endif // WORM_H
