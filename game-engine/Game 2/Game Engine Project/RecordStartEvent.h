#pragma once
#include "Event.h"
#include "ReplayViewer.h"
class RecordStartEvent :
    public Event
{
public:
    RecordStartEvent(float timestamp, float relativePriority, float age);
};

