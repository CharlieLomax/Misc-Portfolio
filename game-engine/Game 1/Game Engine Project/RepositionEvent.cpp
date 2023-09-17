#include "RepositionEvent.h"

RepositionEvent::RepositionEvent(float timestamp, float relativePriority, float age, GameObject* object, sf::Vector2f pos) : Event("Reposition", timestamp, relativePriority, age)
{
	this->object = object;
	this->pos = pos;
}

GameObject* RepositionEvent::getObject()
{
	return object;
}

sf::Vector2f RepositionEvent::getPos()
{
	return pos;
}
