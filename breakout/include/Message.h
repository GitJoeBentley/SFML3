#ifndef MESSAGE_H
#define MESSAGE_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Button.h"

class Message : public sf::RectangleShape
{
public:
    Message(const std::string& txt, sf::Font& font, unsigned int characterSize=30, sf::Vector2f pos = MidWindow, sf::Color color = sf::Color::Yellow, bool continueButton = true);
    virtual ~Message();
    void drawMessage(sf::RenderWindow& window);
    sf::FloatRect getContinueButtonPosition() const;
protected:
    sf::Text text;
    Button* continueButton = nullptr;
};

#endif // MESSAGE_H
