#ifndef TEXTBOX_H
#define TEXTBOX_H
#include <string>
#include <SFML/Graphics.hpp>

class TextBox
{
    public:
        TextBox(sf::RenderWindow& renderWindow, const std::string& str, unsigned fontSize, sf::Vector2f pos, sf::Color textColor = sf::Color::Magenta, sf::Color boxColor = sf::Color::Blue);
        void draw();
        const sf::RectangleShape& getBox() const { return box; }
        void normal();
        void highlight();
        bool isHighlighted() { return highlighted; }
        std::string getStr() const { return str; }
        void flash(sf::Color color = sf::Color::Green);

    private:
        static sf::Font font;
        sf::RenderWindow& window;
        const std::string str;
        unsigned fontSize;
        sf::Text text;
        sf::RectangleShape box;
        sf::Color textColor;
        sf::Color boxColor;
        bool highlighted = false;
};

#endif // TEXTBOX_H
