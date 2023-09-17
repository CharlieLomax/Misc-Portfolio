#pragma once
#include "Mobile.h"
class Paddle :
    public Mobile
{
public:
    Paddle(sf::Vector2f size, sf::Vector2f spawn);
    Paddle(float sizeX, float sizeY, float spawnX, float spawnY);
};

