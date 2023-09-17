#pragma once
#include "Mobile.h"
class Ball :
    public Mobile
{
public:
    Ball();
    Ball(sf::Vector2f size, sf::Vector2f spawn, sf::Vector2f velocity);
    Ball(float sizeX, float sizeY, float spawnX, float spawnY, float velocityX, float velocityY);

private:
    void bounceSide();
    void bounceVertical();
};

