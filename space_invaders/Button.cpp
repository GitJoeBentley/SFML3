#include "Button.h"

Button::Button(sf::Vector2f buttonSize, sf::Vector2f pos, const std::string& textStr, sf::Font& font, unsigned int textSize)
    : sf::RectangleShape(buttonSize), text(font, textStr, textSize)
{
    setPosition(pos);
    setFillColor(color);
    setOutlineThickness(1.0f);
    setOutlineColor(sf::Color::Green);
    sf::FloatRect buttonRect = getLocalBounds();
    setOrigin(sf::Vector2f(buttonRect.position.x + buttonRect.size.x / 2.0f, buttonRect.position.y + buttonRect.size.y / 2.0f));
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
    text.setPosition(pos);
}

void Button::drawit(sf::RenderWindow& window)
{
    window.draw(*this);
    window.draw(text);
}

void Button::highlight()
{
    setFillColor(highlightedColor);
    text.setFillColor(sf::Color::Black);
    setOutlineColor(sf::Color(0x004844ff));
    setOutlineThickness(3.0f);
}

void Button::normal()
{
    setFillColor(color);
    text.setFillColor(sf::Color::White);
    setOutlineColor(sf::Color::Magenta);
    setOutlineThickness(5.0f);
}

void Button::setColor(sf::Color buttColor, sf::Color textColor)
{
    color = buttColor;
    setFillColor(buttColor);
    text.setFillColor(textColor);
    setOutlineColor(sf::Color::Red);
    setOutlineThickness(3.0f);
}
