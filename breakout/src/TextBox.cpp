#include "TextBox.h"
#include "constants.h"
#include <iostream>

TextBox::TextBox(const std::string& str, sf::Font& font, unsigned int textSize, sf::Color color, sf::Vector2f pos)
: textStr(str), text(font, str, textSize)
{
    text.setFillColor(color);
    text.setPosition(pos);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
    setPosition(pos);
    setSize(sf::Vector2f(1.1f * textRect.size.x, 1.2f * textRect.size.y));
    setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
    setOutlineColor(Blue);
    setOutlineThickness(4.0f);
    setFillColor(sf::Color::Black);
}

void TextBox::drawTB(sf::RenderWindow& window)
{
    window.draw(*this);
    window.draw(text);
}
