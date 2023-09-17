#include "DeathZone.h"

DeathZone::DeathZone(sf::Vector2f size, sf::Vector2f position) : GameObject(new DeathCollision(), NULL, new HiddenObject(size, position))
{
}

DeathZone::DeathZone(float sizeX, float sizeY, float positionX, float positionY) : DeathZone(sf::Vector2f(sizeX, sizeY), sf::Vector2f(positionX, positionY))
{
}
