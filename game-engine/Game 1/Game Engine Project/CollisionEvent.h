#pragma once
#include "Event.h"
class CollisionEvent :
    public Event
{
public:
    enum Direction {
        LEFT,
        RIGHT,
        UP,
        DOWN,
        CLEAR
    };

    CollisionEvent(float timestamp, float relativePriority, float age, Direction dir, Mobile* obj, GameObject* other, bool reposition = true);
    Direction getDirection();
    Mobile* getObj();
    GameObject* getOther();
    bool getReposition();

private:
    Direction dir;
    Mobile* obj;
    GameObject* other;
    bool reposition;
};

