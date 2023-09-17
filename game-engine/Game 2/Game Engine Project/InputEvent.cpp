#include "InputEvent.h"

InputEvent::InputEvent(float timestamp, float relativePriority, float age, sf::Keyboard::Key key, bool release) : Event("Input", timestamp, relativePriority, age)
{
	this->key = key;
	this->release = release;
}

sf::Keyboard::Key InputEvent::getKey()
{
	return key;
}

bool InputEvent::isRelease()
{
	return release;
}
