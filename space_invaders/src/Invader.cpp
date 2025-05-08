#include "Constants.h"
#include "Invader.h"

Invader::Invader()
    : sf::RectangleShape(InvaderSize), status(Visible), explosionCount(0U)
{
    setOrigin(sf::Vector2f(getLocalBounds().size.x/2.0f,getLocalBounds().size.y / 2.0f));
}

Invader::~Invader()
{ }
