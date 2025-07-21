#ifndef NUMBERCIRCLE_H
#define NUMBERCIRCLE_H
#include <string>
#include <SFML/Graphics.hpp>

class NumberCircle
{
    public:
        NumberCircle(sf::RenderWindow& renderWindow, const std::string& str,
                     sf::Vector2f pos, sf::Color textColor = sf::Color::Magenta, sf::Color circleColor = sf::Color::Blue);
        void draw();
        const sf::CircleShape& getCircle() const { return circle; }
        sf::Vector2f getCirclePosition() const { return circle.getPosition(); }
        void normal();
        void highlight();
        bool isHighlighted() { return highlighted; }
        std::string getStr() const { return str; }
        void flash(sf::Color color = sf::Color::Green);
        void emphasize();
        operator int() const { return std::stoi(str); }

    private:
        static sf::Font font;
        sf::RenderWindow& window;
        const std::string str;
        sf::Text text;
        sf::CircleShape circle;
        sf::Color textColor;
        sf::Color circleColor;
        bool highlighted = false;
};

#endif // NUMBERCIRCLE_H
