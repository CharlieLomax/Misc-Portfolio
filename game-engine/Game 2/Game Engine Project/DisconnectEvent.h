#pragma once
#include "Event.h"
class DisconnectEvent :
    public Event
{
public:
    DisconnectEvent(float timestamp, float relativePriority, float age, int id);
    int getId();

private:
    int id;
};

