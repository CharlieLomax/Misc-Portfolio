#include "DeathEvent.h"

DeathEvent::DeathEvent(float timestamp, float relativePriority, float age, Mobile* object) : Event("Death", timestamp, relativePriority, age)
{
	this->object = object;
}

Mobile* DeathEvent::getObject()
{
	return object;
}
