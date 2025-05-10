#ifndef BOMB_H
#define BOMB_H

#include <utility>
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "Invaders.h"
//#include "Sound.h"
#include "Gun.h"
#include "Explosion.h"
//#include "Shield.h"

// class Invaders;  // forward declaration

class Sound;
class Shield;


class Bomb : public sf::Sprite
{
public:
    Bomb(sf::Texture& texture, int imageIndex = 4);
    Bomb(sf::Texture& texture, float Xcoordinate);   // bomb targeted at gun location
    void moveBombToStartPosition(const Invaders& invaders);
    bool move();
    bool hitByBullet(Bullet& bullet);
    float bottomPosition() const;
    sf::Vector2f nosePosition() const;
    float leftSidePosition() const;
    float rightSidePosition() const;
    bool manage(Sound& sound, Gun& gun, Explosion& explosion, int& gunInPlay, bool& gameOver, Shield* shields);

private:
    int imageIndex;
    sf::Vector2f position;
};

#endif // BOMB_H
