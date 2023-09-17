#include "SpawnEvent.h"

SpawnEvent::SpawnEvent(float timestamp, float relativePriority, float age, Mobile* obj) : Event("Spawn", timestamp, relativePriority, age)
{
	this->obj = obj;
}

Mobile* SpawnEvent::getObj()
{
	return obj;
}
