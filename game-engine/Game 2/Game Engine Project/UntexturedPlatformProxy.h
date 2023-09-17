#include "GameObject.h"
#include "MovingCollision.h"
#include "RenderRect.h"
#pragma once
class UntexturedPlatformProxy :
    public GameObject
{
public:
    UntexturedPlatformProxy(sf::Vector2f size, sf::Vector2f position, sf::Vector2f velocity, sf::Color color, int serverID);

private:
    int serverID;
};

