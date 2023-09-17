#pragma once
#include "Event.h"
class ConnectEvent :
    public Event
{
public:
    ConnectEvent(float timestamp, float relativePriority, float age, int id);
    int getId();

private:
    int id;
};

