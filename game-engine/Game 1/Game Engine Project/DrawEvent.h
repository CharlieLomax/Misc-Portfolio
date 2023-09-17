#pragma once
#include "Event.h"
class DrawEvent :
	public Event
{
public:
	DrawEvent(float timestamp, float relativePriority, float age);
};

