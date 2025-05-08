#include "CosmicDustParticle.h"
#include <iostream>

CosmicDustParticle::CosmicDustParticle()
    : particle(StarTexture)
{
    setAttributes();
}

void CosmicDustParticle::draw(sf::RenderWindow& window)
{
    window.draw(particle);
}

void CosmicDustParticle::setAttributes()
{
    int bgX = static_cast<int>(BackgroundSize.x);
    int bgY = static_cast<int>(BackgroundSize.y);
    float xPos = static_cast<float>(rand() % bgX);
    float yPos = static_cast<float>(rand() % bgY);
    sf::Vector2f pos = sf::Vector2f(xPos, yPos);
    particle.setPosition(pos);
    float scale = 0.01f / (rand() % 3 + 1);
    particle.setScale(sf::Vector2f(scale,scale));
    // std::cout << "Particle location: " << particlePtr->getPosition().x << " " << particlePtr->getPosition().y << std::endl;
    // Random Color every 4th particle has a color
    if (rand()%4 == 0)
    {
        particle.setColor(sf::Color(128+rand()%128,128+rand()%128,128+rand()%128,255));
    }
}
