#include "PlatformMove.h"
#include "GameObject.h"
#include "EventManager.h"
#include "Timeline.h"
#include "MoveEvent.h"

extern Timeline globalTime;

sf::Vector2f PlatformMove::hat(sf::Vector2f vector) {
	return vector / sqrt((vector.x * vector.x) + (vector.y * vector.y));
}

PlatformMove::PlatformMove(float moveSpeed, std::vector<sf::Vector2f> turnsVector)
{
	speed = moveSpeed;
	turns = turnsVector;
	nextTurn = turns.begin();
}

void PlatformMove::move(GameObject* object, float delta_t) {
	// If the platform has reached the next turn, change direction
	if ((object->getBox().left >= nextTurn->x && object->getVelocity().x > 0) || (object->getBox().left <= nextTurn->x && object->getVelocity().x < 0) || (object->getBox().top >= nextTurn->y && object->getVelocity().y > 0) || (object->getBox().top <= nextTurn->y && object->getVelocity().y < 0)) {

		// Save the next point in the vector
		std::vector<sf::Vector2f>::iterator next = nextTurn + 1;

		// If the next point is past the end of the vector, return to the beginning of the vector
		if (next == turns.end()) {
			next = turns.begin();
		}

		// Recalculate velocity based on the next point
		object->setVelocity(speed * hat(*next - *nextTurn));

		object->setPosition(*nextTurn);

		// Update the next turn
		nextTurn = next;
	}

	if (object->getVelocity() != sf::Vector2f(0, 0) && delta_t != 0)
		EventManager::instance()->raise(new MoveEvent(globalTime.getTime(), 0.f, 0.f, object, delta_t * object->getVelocity()));
}