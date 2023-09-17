#include "CollisionHandler.h"

CollisionHandler::CollisionHandler() : EventHandler(std::list<std::string>({ "Collision" }))
{
}

void CollisionHandler::onEvent(std::shared_ptr<Event> e)
{
	if (e->getType() == "Collision") {
		CollisionEvent event  = *std::static_pointer_cast<CollisionEvent>(e);
		Mobile* obj = event.getObj();
		sf::FloatRect box;
		if(event.getOther() != NULL)
			box = event.getOther()->getBox();
		sf::FloatRect objBox = obj->getBox();
		
		switch (event.getDirection()) {
		case CollisionEvent::Direction::LEFT:
			obj->collideLeft();
			if (event.getReposition())
				obj->setPosition(box.left + box.width, objBox.top);
			break;

		case CollisionEvent::Direction::RIGHT:
			event.getObj()->collideRight();
			if (event.getReposition())
				obj->setPosition(box.left - objBox.width, objBox.top);
			break;

		case CollisionEvent::Direction::UP:
			event.getObj()->collideUp();
			if (event.getReposition())
				obj->setPosition(objBox.left, box.top + box.height);
			break;

		case CollisionEvent::Direction::DOWN:
			event.getObj()->collideDown();
			if (event.getReposition())
				obj->setPosition(objBox.left, box.top - objBox.height);
			break;

		case CollisionEvent::Direction::CLEAR:
			event.getObj()->clearCollision();
			break;
		}
	}
}
