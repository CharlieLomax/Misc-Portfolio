#include "Barrier.h"
#include "PlatformCollision.h"
#include "HiddenObject.h"

Barrier::Barrier(sf::Vector2f size, sf::Vector2f position) : GameObject(new PlatformCollision(), NULL, new HiddenObject(size, position))
{
}

Barrier::Barrier(float sizeX, float sizeY, float positionX, float positionY) : Barrier(sf::Vector2f(sizeX, sizeY), sf::Vector2f(positionX, positionY))
{
}
