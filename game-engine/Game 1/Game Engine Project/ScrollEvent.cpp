#include "ScrollEvent.h"

ScrollEvent::ScrollEvent(float timestamp, float relativePriority, float age, float distance) : Event("Scroll", timestamp, relativePriority, age)
{
	this->distance = distance;
}

float ScrollEvent::getDistance()
{
	return distance;
}
