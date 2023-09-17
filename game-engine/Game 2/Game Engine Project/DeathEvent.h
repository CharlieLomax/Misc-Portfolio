#pragma once
#include "Event.h"
class DeathEvent :
    public Event
{
public:
    DeathEvent(float timestamp, float relativePriority, float age, Mobile* object);
    Mobile* getObject();

private:
    Mobile* object;
};

