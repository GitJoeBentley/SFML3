#include "Message.h"
#include <iostream>

Message::Message(const std::string& txt, sf::Font& font, unsigned int characterSize, sf::Vector2f pos, sf::Color color, bool contButton)
    : text(font, txt, characterSize)
{
    text.setFillColor(color);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
    text.setPosition(pos);

    setSize(sf::Vector2f(textRect.size.x * 1.2f, textRect.size.y * 1.2f));
    setOutlineColor(sf::Color(0xFF7F27));
    setOutlineThickness(10.0f);
    setFillColor(sf::Color(sf::Color::Black));
    sf::FloatRect rect = getLocalBounds();
    setOrigin(sf::Vector2f(rect.position.x + rect.size.x / 2.0f, rect.position.y + rect.size.y / 2.0f));
    setPosition(pos);
    if (contButton)
    {
        unsigned fontsize = 20u;
        const std::string continueStr("CONTINUE");

        sf::Vector2f buttonSize = sf::Vector2f(continueStr.size() * fontsize * 1.1f, fontsize*1.5f);
        setSize(sf::Vector2f(getSize().x, getSize().y + buttonSize.y * 1.5f));

        continueButton = new Button(buttonSize, sf::Vector2f(getPosition().x, getPosition().y + 0.45f * getSize().y), continueStr, font, fontsize);
        sf::FloatRect continueButtonRect = continueButton->getLocalBounds();
        continueButton->setOrigin(sf::Vector2f(continueButtonRect.position.x + continueButtonRect.size.x / 2.0f, continueButtonRect.position.y + continueButtonRect.size.y / 2.0f));
        continueButton->setColor(sf::Color::Yellow, sf::Color::Black);
    }
}

Message::~Message()
{
    if (continueButton)
    {
        delete continueButton;
        continueButton = nullptr;
    }
}

sf::FloatRect Message::getContinueButtonPosition() const
{
    return continueButton->getGlobalBounds();
}

void Message::drawMessage(sf::RenderWindow& window)
{
    window.draw(*this);
    window.draw(text) ;
    if (continueButton) continueButton->drawButton(window);
}
