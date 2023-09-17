#pragma once
#include "Event.h"
class RepositionEvent :
    public Event
{
public:
    RepositionEvent(float timestamp, float relativePriority, float age, GameObject* object, sf::Vector2f pos);
    GameObject* getObject();
    sf::Vector2f getPos();

private:
    GameObject* object;
    sf::Vector2f pos;
};

