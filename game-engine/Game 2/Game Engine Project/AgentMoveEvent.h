#pragma once
#include "Event.h"
class AgentMoveEvent :
    public Event
{
public:
    AgentMoveEvent(float timestamp, float relativePriority, float age, Mobile* object, sf::Vector2f distance);
    Mobile* getObject();
    sf::Vector2f getDistance();

private:
    Mobile* object;
    sf::Vector2f distance;
};

