#include "BoundaryCollision.h"
#include "Mobile.h"
#include "EventManager.h"
#include "ScrollEvent.h"
#include "Timeline.h"

extern Timeline globalTime;

BoundaryCollision::BoundaryCollision(float posLeft, float posRight)
{
	this->posLeft = posLeft;
	this->posRight = posRight;
}

void BoundaryCollision::checkCollision(GameObject* object, Mobile* other)
{
	// We only care about the x position for this collision
	float characterPos = other->getPosition().x;
	float characterWidth = other->getBox().width;

	if (characterPos < posLeft) {
		float distance = characterPos - posLeft;
		//RenderObject::moveCamera(distance, 0.f);
		EventManager::instance()->raise(new ScrollEvent(globalTime.getTime(), 0.f, 0.f, distance));
		
		// Move the boundaries whenever the camera moves
		posLeft += distance;
		posRight += distance;
	}

	else if (characterPos + characterWidth > posRight) {
		float distance = characterPos + characterWidth - posRight;
		//RenderObject::moveCamera(distance, 0.f);
		EventManager::instance()->raise(new ScrollEvent(globalTime.getTime(), 0.f, 0.f, distance));

		// Move the boundaries whenever the camera moves
		posLeft += distance;
		posRight += distance;
	}
}
