#pragma once
#include "GameObject.h"
#include "PlatformMove.h"
#include "MovingCollision.h"
#include "RenderRect.h"
#include <vector>

//extern Timeline globalTime;

class UntexturedMovingPlatform :
    public GameObject
{
    public:
        /* Constructor for an untextured moving platform with the given size, position, and speed.
           turnsVector is a collection of sf::Vector2f objects representing the points that the platform will hit on its path. */
        UntexturedMovingPlatform(sf::Vector2f size, sf::Vector2f position, float moveSpeed, std::vector<sf::Vector2f> turnsVector);
        UntexturedMovingPlatform(sf::Vector2f size, sf::Vector2f position, float moveSpeed, std::vector<sf::Vector2f> turnsVector, sf::Color color);
        
    private:
        /* Returns the unit vector of the given sf::Vector2f object. Used in velocity calculations. */
        sf::Vector2f hat(sf::Vector2f);
};

