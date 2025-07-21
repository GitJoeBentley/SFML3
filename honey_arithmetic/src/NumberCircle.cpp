#include "NumberCircle.h"
#include "constants.h"

sf::Font NumberCircle::font(ArialFont);

NumberCircle::NumberCircle(sf::RenderWindow& renderWindow, const std::string& str_,
                           sf::Vector2f pos, sf::Color txtColor, sf::Color circColor)
    : window(renderWindow), str(str_), text(font,str_,NumberFontSize),
      circle(CircleRadius),
      textColor(txtColor), circleColor(circColor)
{
    sf::FloatRect rect = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(rect.position.x + rect.size.x / 2.0f, rect.position.y + rect.size.y / 2.0f));
    text.setFillColor(textColor);
    text.setPosition(pos);

    circle.setFillColor(sf::Color::Black);

    sf::FloatRect localRect = circle.getLocalBounds();
    circle.setOrigin(sf::Vector2f(localRect.position.x + localRect.size.x / 2.0f, localRect.position.y + localRect.size.y / 2.0f));
    circle.setPosition(pos);
    circle.setOutlineThickness(5.0f);
    circle.setOutlineColor(circleColor);
}

void NumberCircle::draw()
{
    window.draw(circle);
    window.draw(text);
}


void NumberCircle::normal()
{
    text.setFillColor(textColor);
    circle.setOutlineColor(circleColor);
    highlighted = false;
}

void NumberCircle::highlight()
{
    text.setFillColor(circleColor);
    circle.setOutlineColor(textColor);
    highlighted = true;
}

void NumberCircle::flash(sf::Color color)
{
    for (int i = 0; i < 20; i++)
    {
        if (i%2)
        {
            text.setFillColor(color);
            circle.setOutlineColor(color);
            draw();
        }
        else
        {
            text.setFillColor(sf::Color::Black);
            circle.setOutlineColor(sf::Color::Black);
            draw();
        }
        sf::sleep(sf::Time(sf::seconds(0.15f)));
        window.display();
    }
    sf::sleep(sf::Time(sf::seconds(1.0f)));
}

void NumberCircle::emphasize()
{
    text.setFillColor(sf::Color::Black);
    circle.setFillColor(sf::Color::Green);
    draw();
    window.display();
    sf::sleep(sf::Time(sf::seconds(3.0f)));
}

