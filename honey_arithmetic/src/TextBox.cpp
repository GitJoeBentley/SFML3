#include "constants.h"
#include "TextBox.h"
#include "Sound.h"
using namespace std;

sf::Font TextBox::font(ArialFont);

TextBox::TextBox(sf::RenderWindow& renderWindow, const string& str_, unsigned fontSize_, sf::Vector2f pos, sf::Color txtColor, sf::Color bxColor)
    : window(renderWindow), str(str_), fontSize(fontSize_), text(font,str_,fontSize_),
      box(sf::Vector2f(str.size() * static_cast<float>(fontSize_) * .75f, 1.5f * static_cast<float>(fontSize_))),
      textColor(txtColor), boxColor(bxColor)
{
    sf::FloatRect rect = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(rect.position.x + rect.size.x / 2.0f, rect.position.y + rect.size.y / 2.0f));
    text.setFillColor(textColor);
    text.setPosition(pos);

    box.setFillColor(sf::Color::Black);

    sf::FloatRect localRect = box.getLocalBounds();
    box.setOrigin(sf::Vector2f(localRect.position.x + localRect.size.x / 2.0f, localRect.position.y + localRect.size.y / 2.0f));
    box.setPosition(pos);
    box.setOutlineThickness(10.0f);
    box.setOutlineColor(boxColor);
}

void TextBox::draw()
{
    window.draw(box);
    window.draw(text);
}

void TextBox::normal()
{
    text.setFillColor(textColor);
    box.setOutlineColor(boxColor);
    highlighted = false;
}

void TextBox::highlight()
{
    text.setFillColor(boxColor);
    box.setOutlineColor(textColor);
    highlighted = true;
}

void TextBox::flash(sf::Color color)
{
    for (int i = 0; i < 20; i++)
    {
        if (i%2)
        {
            text.setFillColor(color);
            box.setOutlineColor(color);
            draw();
        }
        else{
            text.setFillColor(sf::Color::Black);
            box.setOutlineColor(sf::Color::Black);
            draw();
        }
        sf::sleep(sf::Time(sf::seconds(0.15f)));
        window.display();
    }
}
