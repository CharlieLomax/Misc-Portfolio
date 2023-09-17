#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "PlatformCollision.h"
#include "RenderRect.h"

class UntexturedPlatform :
    public GameObject
{
    public:
        /* Constructor for an untextured platform with a given size and position */
        UntexturedPlatform(sf::Vector2f size, sf::Vector2f position);
        UntexturedPlatform(float sizeX, float sizeY, float positionX, float positionY);
};

