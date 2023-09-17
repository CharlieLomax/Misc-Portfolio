#pragma once
#include "Event.h"
class MoveEvent :
    public Event
{
public:
    MoveEvent(float timestamp, float relativePriority, float age, GameObject* object, sf::Vector2f distance);
    GameObject* getObject();
    sf::Vector2f getDistance();

private:
    GameObject* object;
    sf::Vector2f distance;
};

