#include "Tile.h"
#include "GameBoard.h"
#include "Constants.h"


Tile::Tile(sf::Vector2f position)
    : tile(sf::Vector2f(TileWidth,TileHeight)), letter(GameBoard::TheFont, "", 32u)
{
    tile.setPosition(position);
    tile.setOrigin(sf::Vector2f(tile.getLocalBounds().size.x/2.0f,tile.getLocalBounds().size.y / 2.0f));
    tile.setOutlineColor(GreyColor);
    tile.setOutlineThickness(1.0f);

    // Position the letter in the center of the tile
    letter.setPosition(sf::Vector2f(position.x + 22.f,position.y + 14.f));
    letter.setOrigin(sf::Vector2f(tile.getLocalBounds().size.x/2.0f,tile.getLocalBounds().size.y / 2.0f));
    letter.setStyle(sf::Text::Bold);
    clear();
}

void Tile::clear()
{
    tile.setFillColor(sf::Color::White);
    letter.setString(' ');
    letter.setFillColor(sf::Color::Black);
}

Tile Tile::getTile() const
{
    return *this;
}
sf::RectangleShape& Tile::getTile()
{
    return tile;
}

char Tile::getChar() const
{
    return letter.getString()[0];
}

sf::Text& Tile::getLetter()
{
    return letter;
}

void Tile::setLetter(char ltr)
{
    letter.setString(ltr);
}

void Tile::draw(sf::RenderWindow& window)
{
    window.draw(tile);
    window.draw(letter);
}

void Tile::drawTile(sf::RenderWindow& window)
{
    window.draw(tile);
}

void Tile::change_tile_color(sf::Color color)
{
    tile.setFillColor(color);
}

void Tile::moveLeft(float pixels)
{
    tile.setPosition(sf::Vector2f(tile.getPosition().x - pixels,tile.getPosition().y));
    letter.setPosition(sf::Vector2f(letter.getPosition().x - pixels,letter.getPosition().y));
}

void Tile::moveRight(float pixels)
{
    tile.setPosition(sf::Vector2f(tile.getPosition().x + pixels,tile.getPosition().y));
    letter.setPosition(sf::Vector2f(letter.getPosition().x + pixels,letter.getPosition().y));
}

void Tile::moveUp(float pixels)
{
    tile.setPosition(sf::Vector2f(tile.getPosition().x,tile.getPosition().y-pixels));
    letter.setPosition(sf::Vector2f(letter.getPosition().x,letter.getPosition().y-pixels));
}

void Tile::moveVertical(float pixels)
{
    tile.setPosition(sf::Vector2f(tile.getPosition().x,tile.getPosition().y + pixels));
    letter.setPosition(sf::Vector2f(letter.getPosition().x,letter.getPosition().y + pixels));
}

void Tile::flip(float percent)
{
    tile.setScale(sf::Vector2f(1.0f,percent));
    letter.setScale(sf::Vector2f(1.0f,percent));
}
