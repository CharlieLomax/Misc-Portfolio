#pragma once
#include "GameObject.h"
class SelfDestructBlock :
    public GameObject
{
public:
    SelfDestructBlock(sf::Vector2f size, sf::Vector2f position, sf::Color color = sf::Color::Red);
};

