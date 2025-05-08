#ifndef COSMICDUSTPARTICLE_H
#define COSMICDUSTPARTICLE_H

#include <SFML/Graphics.hpp>

extern sf::Vector2f BackgroundSize;
extern sf::Texture StarTexture;

class CosmicDustParticle
{
public:
    CosmicDustParticle();
    virtual ~CosmicDustParticle() = default;
    sf::Sprite getParticle() { return particle; }
    sf::Vector2f get_position() const { return position; }
    void draw(sf::RenderWindow& window);
    void setAttributes();
protected:

private:
    sf::Sprite particle;
    sf::Vector2f position;
};

#endif // COSMICDUSTPARTICLE_H
