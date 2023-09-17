#include "AgentMoveEvent.h"
AgentMoveEvent::AgentMoveEvent(float timestamp, float relativePriority, float age, Mobile* object, sf::Vector2f distance) : Event("AgentMove", timestamp, relativePriority, age)
{
	this->object = object;
	this->distance = distance;
}

Mobile* AgentMoveEvent::getObject()
{
	return object;
}

sf::Vector2f AgentMoveEvent::getDistance()
{
	return distance;
}