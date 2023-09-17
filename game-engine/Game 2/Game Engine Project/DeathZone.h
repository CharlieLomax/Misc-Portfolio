#pragma once
#include "GameObject.h"
#include "HiddenObject.h"
#include "DeathCollision.h"
class DeathZone :
    public GameObject
{
public:
    DeathZone(sf::Vector2f size, sf::Vector2f position);
    DeathZone(float sizeX, float sizeY, float positionX, float positionY);
};

