#include <iostream>
#include <cstdlib>
#include <cmath>
#include "Bomb.h"
#include "Sound.h"
#include "Shield.h"
#include "constants.h"

using std::cout;
using std::cerr;
using std::endl;


Bomb::Bomb(sf::Texture& texture, int index)
    : sf::Sprite(texture), imageIndex(index<3?index:rand()%3)
{
    setOrigin(sf::Vector2f(getLocalBounds().size.x/2.0f,getLocalBounds().size.y / 2.0f));
    sf::IntRect bombImageLocationInTexture;
    bombImageLocationInTexture.size.y = static_cast<int>(BombSize.y);
    bombImageLocationInTexture.size.x = static_cast<int>(BombSize.x);
    bombImageLocationInTexture.position.x = 0;
    bombImageLocationInTexture.position.y = imageIndex * texture.getSize().y / 3;
    setTextureRect(bombImageLocationInTexture);
}

// A targeted bomb drops just above the gun (slightly offset, left or right))
Bomb::Bomb(sf::Texture& texture, float Xcoordinate)
    : sf::Sprite(texture), imageIndex(0),
      position(sf::Vector2f(rand()%2 ? Xcoordinate + 10.0f : Xcoordinate - 10.0f, 0.5f * MainWindowHeight))
{
    // set texture for bomb
    if (!texture.loadFromFile(BombImageFile))
        cerr << "Unable to load file " << BombImageFile << endl;
    setTexture(texture);
    sf::IntRect bombImageLocationInTexture;
    bombImageLocationInTexture.size.y = static_cast<int>(BombSize.y);
    bombImageLocationInTexture.size.x = static_cast<int>(BombSize.x);
    bombImageLocationInTexture.position.x = 0;
    bombImageLocationInTexture.position.y = 0;
    setTextureRect(bombImageLocationInTexture);
    setOrigin(sf::Vector2f(getLocalBounds().size.x/2.0f,getLocalBounds().size.y / 2.0f));
    setPosition(position);
}

void Bomb::moveBombToStartPosition(const Invaders& invaders)
{
    int bomberCol = rand() % Invaders::NumInvaders.y;
    int bomberRow = invaders.getMaxIndexOfVisibleInvaderByColumn(bomberCol);
    if (bomberRow != -1)
        position = sf::Vector2f(invaders.getInvaderPosition(bomberRow,bomberCol));
    else
        position = sf::Vector2f(static_cast<float>(MainWindowWidth), static_cast<float>(MainWindowHeight));
}

bool Bomb::move()
{
    int bombWiggle = rand() % 10;
    if (position == sf::Vector2f(static_cast<float>(MainWindowWidth), static_cast<float>(MainWindowHeight)))
    {
        return false;
    }

    sf::IntRect bombImageLocationInTexture;
    bombImageLocationInTexture = getTextureRect();
    if (bombImageLocationInTexture.position.x == 0)
        bombImageLocationInTexture.position.x = 12;
    else
        bombImageLocationInTexture.position.x = 0;

    setTextureRect(bombImageLocationInTexture);
    position.y += BombSpeed;

    // Wiggle the bomb once in a while
    if (bombWiggle == 1) position.x += BombSpeed;
    if (bombWiggle == 2) position.x -= BombSpeed;

    setPosition(position);
    if (position.y > 0.95f * MainWindowHeight)
        return false;
    return true;
}

bool Bomb::hitByBullet(Bullet& bullet)
{
    bool hit;
    if ((position.y - bullet.getPosition().y) > BombSize.y / 2.0f)
        return false;
    float diffX = fabs(position.x - bullet.getPosition().x);
    float diffY = fabs(position.y - bullet.getPosition().y);
    hit = diffX < 5.0f && diffY < 5.0f;
    return hit;
}

float Bomb::bottomPosition() const
{
    return position.y + BombSize.y / 2.0f;
}

sf::Vector2f Bomb::nosePosition() const
{
    return sf::Vector2f(position.x, position.y + BombSize.y / 2.0f);
}

float Bomb::leftSidePosition() const
{
    return position.x - BombSize.x / 4.0f;
}

float Bomb::rightSidePosition() const
{
    return position.x + BombSize.x / 4.0f;
}

// Return false if the bomb:
//   moves below the shield
//   hits the shield
//   if hits the gun
bool Bomb::manage(Sound& sound, Gun& gun, Explosion& explosion, int& gunInPlay, bool& gameOver, Shield* shields)
{
    if (!move())
    {
        sound.stop("Bomb");
        sound.start("Invaders");
        return false;
    }
    else
    {
        /// Bomb hits gun?
        if (gun.hitByBomb(*this))   // gun hit by bomb?
        {
            sound.stop("Bomb");
            sound.start("BombExplosion");
            explosion.startExplosion(gun.getPosition());
            if (gunInPlay == 2) gameOver = true;
            return false;
        }
        /// Bomb hits shield
        else if (bottomPosition() >= 0.9f * MainWindowHeight - ShieldHeight / 2.0f)
        {
            sf::Vector2f shieldPos;
            for (int i = 0; i < 3; ++i)
            {
                shieldPos = shields[i].getShield().getPosition();
                if (shields[i].isAlignedWithBomb(*this) && bottomPosition() > shields[i].topOfShield() - 10.f)
                {
                    sf::Vector2f shieldPositionToExamine = sf::Vector2f(32 + (nosePosition().x - shieldPos.x),22.5f - (shieldPos.y - nosePosition().y));

                    if (shields[i].hitByBomb(shieldPositionToExamine))
                    {
                        sound.start("Explosion");
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
