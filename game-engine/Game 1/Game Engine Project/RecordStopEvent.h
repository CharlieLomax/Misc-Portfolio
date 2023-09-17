#pragma once
#include "Event.h"
class RecordStopEvent :
    public Event
{
public:
    RecordStopEvent(float timestamp, float relativePriority, float age);
};

