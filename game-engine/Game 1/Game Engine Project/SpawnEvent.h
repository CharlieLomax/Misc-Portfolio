#pragma once
#include "Event.h"
class SpawnEvent :
    public Event
{
public:
    SpawnEvent(float timestamp, float relativePriority, float age, Mobile* obj);
    Mobile* getObj();

private:
    Mobile* obj;
};

