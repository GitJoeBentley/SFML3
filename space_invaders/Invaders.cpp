#include <iostream>
#include <cmath>
#include <cstdlib>
#include "Constants.h"
#include "Invaders.h"
#include "Gun.h"

using std::cout;
using std::cerr;
using std::endl;

const sf::Vector2u Invaders::NumInvaders{5,12};

Invaders::Invaders()
    : direction(Right), speed(InvaderSpeed), countVisible(NumInvaders.x * NumInvaders.y)
{
    if (!InvaderTexture.loadFromFile(InvaderImageFile))
        cerr << "Unable to load file " << InvaderImageFile << endl;
    sf::IntRect invaderImageLocationInTexture;
    invaderImageLocationInTexture.size.y = static_cast<int>(InvaderSize.y);
    invaderImageLocationInTexture.size.x = static_cast<int>(InvaderSize.x);
    invaderImageLocationInTexture.position.x = 0;

    for (auto row = 0u; row < NumInvaders.x; ++row)
    {
        for (auto col = 0u; col < NumInvaders.y; ++col)
        {
            //cout << "row: " << row << "  col:  " << col << "  position.y: " << row * InvaderSize.y << endl;
            // set texture for Invader (sf::RectangleShape)
            invaderImageLocationInTexture.position.y = static_cast<int>(row * InvaderSize.y);
            invader[row][col].setTextureRect(invaderImageLocationInTexture);
            invader[row][col].setTexture(&InvaderTexture);
        }
    }
}

void Invaders::moveInvadersToStartPosition()
{
    for (auto row = 0u; row < NumInvaders.x; ++row)
    {
        for (auto col = 0u; col < NumInvaders.y; ++col)
        {
            invader[row][col].setPosition(sf::Vector2f(100.f + 2.f * InvaderSize.x*col, 125.f + 2.f * InvaderSize.y*row));
        }
    }
}

sf::Vector2f Invaders::getInvaderPosition(int row, int col) const
{
    return invader[row][col].getPosition();
}

bool Invaders::move()
{
    bool moveDownFlag = false;
    sf::IntRect invaderImageLocationInTexture;
    sf::Vector2f invaderCurrentPosition;
    if (invaderImageLocationInTexture.position.x == 0)
        invaderImageLocationInTexture.position.x = 32;
    else
        invaderImageLocationInTexture.position.x = 0;

    if (invader[0][getIndexOfRightmostInvaderColumn()].getPosition().x > MainWindowWidth - 2 * InvaderSize.x)
    {
        direction = Left;
        moveDownFlag = true;
    }
    if (invader[0][getIndexOfLeftmostInvaderColumn()].getPosition().x < InvaderSize.x)
    {
        direction = Right;
        moveDownFlag = true;;
    }

    for (auto row = 0u; row < NumInvaders.x; ++row)
    {
        for (auto col = 0u; col < NumInvaders.y; ++col)
        {
            invaderImageLocationInTexture = invader[row][col].getTextureRect();
            if (invaderImageLocationInTexture.position.x == 0)
                invaderImageLocationInTexture.position.x = 32;
            else
                invaderImageLocationInTexture.position.x = 0;

            invader[row][col].setTextureRect(invaderImageLocationInTexture);
            invaderCurrentPosition = getInvaderPosition(row,col);
            if (direction == Right)
                invaderCurrentPosition.x += speed;
            else
                invaderCurrentPosition.x -= speed;
            if (moveDownFlag)
            {
                // move down 10 pixels
                invaderCurrentPosition.y += 11.0f;
            }
            invader[row][col].setPosition(invaderCurrentPosition);
        }
    }

    return true;
}

float Invaders::getPositionOfBottomVisibleInvader() const
{
    float bottomPosition = 0;
    for (auto row = 0u; row < NumInvaders.x; ++row)
    {
        for (auto col = 0u; col < NumInvaders.y; ++col)
        {
            if (invader[row][col].isVisible() && invader[row][col].getPosition().y > bottomPosition)
            {
                bottomPosition = invader[row][col].getPosition().y;
                break;
            }
        }
    }
    return bottomPosition;
}


void Invaders::updateStatus()
{
    for (auto row = 0u; row < NumInvaders.x; ++row)
    {
        for (auto col = 0u; col < NumInvaders.y; ++col)
        {
            if (invader[row][col].getStatus() == Invader::Hit && invader[row][col].getExplosionCount() > 0)
                invader[row][col].incrementExplosionCount();
        }
    }
}

void Invaders::draw(sf::RenderWindow& window)
{
    for (auto row = 0u; row < NumInvaders.x; ++row)
    {
        for (auto col = 0u; col < NumInvaders.y; ++col)
        {
            if (invader[row][col].getStatus() != Invader::Invisible)
                window.draw(invader[row][col]);
        }
    }
}

// The return value represents the number of points for the invader hit
int Invaders::invaderHitByBullet(Bullet& bullet)
{
    sf::Vector2f invaderPosition;
    sf::Vector2f bulletPosition = bullet.getPosition();

    float deltaX, deltaY;
    float distanceFromBulletToInvader;

    sf::Vector2f invaderSize = invader[0][0].getSize();
    float minimumDeltaForAHit = invaderSize.x/2;

    for (int row = NumInvaders.x-1; row >= 0; --row)
    {
        for (auto col = 0u; col < NumInvaders.y; ++col)
        {
            // If the invader is not visible, skip the hit check
            if (invader[row][col].getStatus() != Invader::Visible) continue;

            // Calculate distance between bullet and invader
            invaderPosition = getInvaderPosition(row,col);
            deltaX = std::fabs(invaderPosition.x - bulletPosition.x);
            deltaY = std::fabs(invaderPosition.y - bulletPosition.y);
            distanceFromBulletToInvader = std::sqrt(deltaX*deltaX + deltaY*deltaY);

            if (distanceFromBulletToInvader <= minimumDeltaForAHit)
            {
                // set explosion texture
                sf::IntRect explosionImageLocationInTexture;
                explosionImageLocationInTexture.size.y = static_cast<int>(InvaderSize.y);
                explosionImageLocationInTexture.size.x = static_cast<int>(InvaderSize.x);
                explosionImageLocationInTexture.position.x = static_cast<int>((std::rand()%2) * 32.0f);
                explosionImageLocationInTexture.position.y = static_cast<int>(NumInvaders.x * InvaderSize.y);

                invader[row][col].setTextureRect(explosionImageLocationInTexture);
                invader[row][col].setTexture(&InvaderTexture);
                invader[row][col].setHitStatus();
                invader[row][col].incrementExplosionCount();
                increaseSpeed();
                --countVisible;
                return 5 - row;  // return the number of points for an invader
            }
        }
    }
    return 0;
}

void Invaders::addRandomInvader()
{
    static int lastCol = -1;
    int row = rand()%NumInvaders.x;
    int col = rand()%NumInvaders.y;
    // do not place random invader in the same column as the last one
    if (col == lastCol)
        return;
    Invader& randomInvader = invader[row][col];
    randomInvader.setStatus(Invader::Visible);
    randomInvader.resetExplosionCount();
    randomInvader.setPosition(sf::Vector2f(randomInvader.getPosition().x,100.0f));

    // reset the texture
    sf::IntRect invaderImageLocationInTexture;
    invaderImageLocationInTexture.size.y = static_cast<int>(InvaderSize.y);
    invaderImageLocationInTexture.size.x = static_cast<int>(InvaderSize.x);
    invaderImageLocationInTexture.position.x = 0;
    invaderImageLocationInTexture.position.y = static_cast<int>(row * InvaderSize.y);
    randomInvader.setTextureRect(invaderImageLocationInTexture);
    randomInvader.setTexture(&InvaderTexture);
    ++countVisible;
    lastCol = col;
}

int Invaders::getMaxIndexOfVisibleInvaderByColumn(int column) const
{
    int maxIndex = -1;
    for (unsigned row = 0; row < NumInvaders.x; ++row)
        if (invader[row][column].isVisible())
            maxIndex = row;
    return maxIndex;
}

int Invaders::getMaxIndexOfVisibleInvaders() const
{
    unsigned maxIndex = -1;
    for (int row = NumInvaders.x - 1; row >= 0; --row)
    {
        for (auto col = 0u; col < NumInvaders.y; ++col)
        {
            if (invader[row][col].isVisible())
                return row;
        }
    }
    return maxIndex;
}

int Invaders::getIndexOfRightmostInvaderColumn() const
{
    for (int rightmostColumn = NumInvaders.y - 1; rightmostColumn >= 0; --rightmostColumn)
    {
        if (getMaxIndexOfVisibleInvaderByColumn(rightmostColumn) != -1)
            return rightmostColumn;
    }
    return -1;
}

int Invaders::getIndexOfLeftmostInvaderColumn() const
{
    for (unsigned leftmostColumn = 0u; leftmostColumn < NumInvaders.y - 1; ++leftmostColumn)
    {
        if (getMaxIndexOfVisibleInvaderByColumn(leftmostColumn) != -1)
            return leftmostColumn;
    }
    return -1;
}
