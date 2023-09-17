#include "UntexturedPlatform.h"
#include "Character.h"

UntexturedPlatform::UntexturedPlatform(sf::Vector2f size, sf::Vector2f position) : GameObject(new PlatformCollision(), NULL, new RenderRect(size, position)) {
    
}

UntexturedPlatform::UntexturedPlatform(float sizeX, float sizeY, float positionX, float positionY) : UntexturedPlatform(sf::Vector2f(sizeX, sizeY), sf::Vector2f(positionX, positionY))
{
}

