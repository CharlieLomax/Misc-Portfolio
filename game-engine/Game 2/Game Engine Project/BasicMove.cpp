#include "BasicMove.h"
#include "GameObject.h"
#include "EventManager.h"
#include "Timeline.h"
#include "MoveEvent.h"

BasicMove::BasicMove()
{
}

void BasicMove::move(GameObject* object, float delta_t)
{
	if (object->getVelocity() != sf::Vector2f(0, 0) && delta_t != 0)
		EventManager::instance()->raise(new MoveEvent(globalTime.getTime(), 0.f, 0.f, object, delta_t * object->getVelocity()));
}
