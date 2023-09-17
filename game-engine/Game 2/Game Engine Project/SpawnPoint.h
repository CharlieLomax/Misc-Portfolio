#pragma once
#include "GameObject.h"
#include "SpawnCollision.h"
class SpawnPoint :
    public GameObject
{
public:
    SpawnPoint(sf::Vector2f size, sf::Vector2f position, sf::Vector2f point);
    SpawnPoint(float sizeX, float sizeY, float positionX, float positionY, float pointX, float pointY);
};

