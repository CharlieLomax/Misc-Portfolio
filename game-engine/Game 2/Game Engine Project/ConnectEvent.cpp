#include "ConnectEvent.h"

ConnectEvent::ConnectEvent(float timestamp, float relativePriority, float age, int id) : Event("Connect", timestamp, relativePriority, age)
{
	this->id = id;
}

int ConnectEvent::getId()
{
	return id;
}
