#include "DeathCollision.h"
#include "Mobile.h"
#include "EventManager.h"
#include "DeathEvent.h"
#include "Timeline.h"

extern Timeline globalTime;

void DeathCollision::checkCollision(GameObject* object, Mobile* other)
{
	if (object->getBox().intersects(other->getBox())) {
		EventManager::instance()->raise(new DeathEvent(globalTime.getTime(), 0.f, 0.f, other));
	}
}
