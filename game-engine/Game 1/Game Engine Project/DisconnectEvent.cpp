#include "DisconnectEvent.h"

DisconnectEvent::DisconnectEvent(float timestamp, float relativePriority, float age, int id) : Event("Disconnect", timestamp, relativePriority, age)
{
	this->id = id;
}

int DisconnectEvent::getId()
{
	return id;
}