#include "MoveEvent.h"

MoveEvent::MoveEvent(float timestamp, float relativePriority, float age, GameObject* object, sf::Vector2f distance) : Event("Move", timestamp, relativePriority, age)
{
	this->object = object;
	this->distance = distance;
}

GameObject* MoveEvent::getObject()
{
	return object;
}

sf::Vector2f MoveEvent::getDistance()
{
	return distance;
}
