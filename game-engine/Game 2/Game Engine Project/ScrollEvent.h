#pragma once
#include "Event.h"
class ScrollEvent :
    public Event
{
public:
    ScrollEvent(float timestamp, float relativePriority, float age, float distance);
    float getDistance();

private:
    float distance;
};

