#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <bitset>
#include "Shield.h"

using std::cerr;
using std::endl;

Shield::Shield()
    : shieldData(new uint64_t[ShieldHeight]),
      shieldImage(new sf::Image(ShieldSize)),
      shieldTexture(new sf::Texture()),
      shield(nullptr)
{
    static uint64_t InitialShieldData[ShieldHeight];
    static bool GotShieldData = false;
    static sf::Vector2f ShieldPosition = sf::Vector2f{0.0f, ShieldYPosition};
    ShieldPosition.x += MainWindowWidth / 4.0f;
    if (!GotShieldData)
    {
        getShieldDataFromFile(InitialShieldData);
        GotShieldData = true;
    }
    memcpy(shieldData,InitialShieldData,ShieldHeight * sizeof(uint64_t));
    // create the Shield image
    uint64_t mask;
    for (unsigned y = 0; y < 45; y++)
    {
        mask = 1;
        for (unsigned x = 0; x < 64; x++, mask <<= 1)
        {
            if (mask & shieldData[y])
                shieldImage->setPixel(sf::Vector2u(x,y),GreenPixel);
            else
                shieldImage->setPixel(sf::Vector2u(x,y),sf::Color::Transparent);
        }
    }
    if (!shieldTexture->loadFromImage(*shieldImage))
        std::cerr << "Unable to load shield image into texture in function: Shield::Shield()" << std::endl;
    shield = new sf::Sprite(*shieldTexture);
    sf::FloatRect rect = shield->getLocalBounds();
    sf::Vector2f shieldSize = rect.size;
    shield->setOrigin(sf::Vector2f(shieldSize.x/2.0f, ShieldHeight / 2.0f));
    shield->setPosition(ShieldPosition);
}

Shield::~Shield()
{
    delete shield;
    shield = nullptr;
    delete shieldTexture;
    shieldTexture = nullptr;
    delete shieldImage;
    shieldImage = nullptr;
    delete [] shieldData;
    shieldData = nullptr;
}

// Is the bomb aligned with the shield?
bool Shield::isAlignedWithBomb(const Bomb& bomb) const
{
    if (bomb.rightSidePosition() <= leftSidePosition())
        return false;
    if (bomb.leftSidePosition() >= rightSidePosition())
        return false;
    return true;
}

void Shield::getShieldDataFromFile(uint64_t* InitialShieldData)
{
    std::string buffer;
    std::ifstream fin(ShieldDataFile);
    if (!fin)
    {
        std::cerr << "Unable to open file " << ShieldDataFile << std::endl;
        std::exit(23);
    }
    for (auto i = 0u; i < ShieldSize.y; i++)
    {
        getline(fin,buffer);
        // remove spaces
        buffer.erase(std::remove(buffer.begin(),buffer.end(),' '),buffer.end());
        // didn't work on Mac g++ InitialShieldData[i] = bitset<64>(buffer).to_ullong();
        InitialShieldData[i] = std::stoull(buffer.c_str(),0,2);
    }
}

bool Shield::hitByBomb(sf::Vector2f position)
{
    std::bitset<64> bits;
    int minXpos = static_cast<int>(position.x - BombSize.x/2 - 1);
    minXpos = minXpos < 0 ? 0 : minXpos;
    int maxXpos = static_cast<int>(position.x + BombSize.x/2 + 1);
    maxXpos = maxXpos > 63 ? 63 : maxXpos;
    bool itsAHit = false;

    // count the bits in the intersecting rectangle
    for (auto row = 0u; row < static_cast<size_t>(position.y + 1) && row < ShieldSize.y; ++row)
    {
        bits.reset();
        bits |= shieldData[row];
        for (auto col = minXpos; col <= maxXpos; ++col)
        {
            if (bits.test(col))
            {
                itsAHit = true;
                break;
            }
        }
    }
    // if it's a hit, remove 5/6 pixels of the pixels in the targeted rectangle (+ a little more)
    if (itsAHit)
    {
        minXpos = minXpos - rand() % 5;
        minXpos = minXpos < 0 ? 0 : minXpos;
        maxXpos = maxXpos + rand() % 5;
        maxXpos = maxXpos > 63 ? 63 : maxXpos;
        unsigned maxYpos = static_cast<unsigned>(position.y + 1u + rand() % 10);

        for (auto row = 0u; row < maxYpos && row < ShieldSize.y; ++row)
        {
            bits.reset();
            bits |= shieldData[row];
            for (auto col = minXpos; col <= maxXpos; ++col)
            {
                if (rand() % 6)
                    bits.reset(col);
            }
            shieldData[row] = bits.to_ullong();
        }
        uint64_t mask;
        for (int y = 0; y < 45; y++)
        {
            mask = 1;
            for (int x = 0; x < 64; x++, mask <<= 1)
            {
                if (mask & shieldData[y])
                    shieldImage->setPixel(sf::Vector2u(x,y),GreenPixel);
                else
                    shieldImage->setPixel(sf::Vector2u(x,y),TransparentPixel);
            }
        }

        if (!shieldTexture->loadFromImage(*shieldImage))
            std::cerr << "Unable to load shield image into texture in function: Shield::hitByBomb()" << std::endl;
        shield->setTexture(*shieldTexture);

    }
    return itsAHit;
}

// Is the bullet aligned with the shield?
bool Shield::isAlignedWithBullet(const Bullet& bullet) const
{
    if (bullet.getPosition().x > rightSidePosition() + BulletRadius)
        return false;
    if (bullet.getPosition().x < leftSidePosition() - BulletRadius)
        return false;
    if (bullet.getPosition().y > bottomOfShield())
        return false;
    return true;
}

bool Shield::hitByBullet(sf::Vector2f position)
{
    bool itsAHit = false;
    std::bitset<64> bits;
    int minXpos = static_cast<int>(position.x - BulletRadius - 1);
    minXpos = minXpos < 0 ? 0 : minXpos;
    int maxXpos = static_cast<int>(position.x + BulletRadius + 1);
    maxXpos = maxXpos > 63 ? 63 : maxXpos;
    int minRow = static_cast<unsigned>(position.y) - 1;
    minRow = minRow >= 0 ? minRow : 0;

    // count the bits in the intersecting rectangle
    for (int row = ShieldSize.y - 1; row >= minRow; --row)
    {
        bits.reset();
        bits |= shieldData[row];
        for (auto col = minXpos; col <= maxXpos; ++col)
        {
            if (bits.test(col))
            {
                itsAHit = true;
                break;
            }
        }
    }
    // if it's a hit, remove 4/5 pixels of the pixels in the targeted rectangle (+ a little more)
    if (itsAHit)
    {
        minXpos = minXpos - rand() % 2;
        minXpos = minXpos < 0 ? 0 : minXpos;
        maxXpos = maxXpos + rand() % 2;
        maxXpos = maxXpos > 63 ? 63 : maxXpos;

        for (int row = ShieldSize.y - 1; row >= 0 && row >= minRow; --row)
        {
            bits.reset();
            bits |= shieldData[row];
            for (auto col = minXpos; col <= maxXpos; ++col)
            {
                if (rand() % 8)
                    bits.reset(col);
            }
            shieldData[row] = bits.to_ullong();
        }
        uint64_t mask;
        for (int y = 0; y < 45; y++)
        {
            mask = 1;
            for (int x = 0; x < 64; x++, mask <<= 1)
            {
                if (mask & shieldData[y])
                    shieldImage->setPixel(sf::Vector2u(x,y),sf::Color(0,255,0,255));
                else
                    shieldImage->setPixel(sf::Vector2u(x,y),sf::Color::Transparent);
            }
        }
        if (!shieldTexture->loadFromImage(*shieldImage))
            std::cerr << "Unable to load shield image" << std::endl;
        shield->setTexture(*shieldTexture, false);
    }
    return itsAHit;
}
