#include "SpawnPoint.h"

SpawnPoint::SpawnPoint(sf::Vector2f size, sf::Vector2f position, sf::Vector2f point) : GameObject(new SpawnCollision(sf::FloatRect(position, size), point), NULL, NULL)
{

}

SpawnPoint::SpawnPoint(float sizeX, float sizeY, float positionX, float positionY, float pointX, float pointY) : SpawnPoint(sf::Vector2f(sizeX, sizeY), sf::Vector2f(positionX, positionY), sf::Vector2f(pointX, pointY))
{
}
