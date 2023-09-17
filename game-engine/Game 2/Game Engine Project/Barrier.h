#pragma once
#include "GameObject.h"
class Barrier :
    public GameObject
{
public:
    Barrier(sf::Vector2f size, sf::Vector2f position);
    Barrier(float sizeX, float sizeY, float positionX, float positionY);
};

