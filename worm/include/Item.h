#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>

extern sf::Vector2f BackgroundSize;

class Item
{
public:
    Item(const sf::Texture& texture, const std::vector<Item*>& itemVector, float sz = 1.0f);
    virtual ~Item() = default;
    const sf::Sprite& getSprite() const
    {
        return sprite;
    }
    void move();
private:
    sf::Sprite sprite;
    sf::Vector2f size;
    const std::vector<Item*>& itemsInExistence;
};

#endif // ITEM_H
