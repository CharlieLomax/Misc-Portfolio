#include "CollisionEvent.h"

CollisionEvent::CollisionEvent(float timestamp, float relativePriority, float age, Direction dir, Mobile* obj, GameObject* other, bool reposition) : Event("Collision", timestamp, relativePriority, age)
{
	this->dir = dir;
	this->obj = obj;
	this->other = other;
	this->reposition = reposition;
}

CollisionEvent::Direction CollisionEvent::getDirection()
{
	return dir;
}

Mobile* CollisionEvent::getObj()
{
	return obj;
}

GameObject* CollisionEvent::getOther()
{
	return other;
}

bool CollisionEvent::getReposition()
{
	return reposition;
}
