#include "AgentMoveHandler.h"
#include "AgentMoveEvent.h"

AgentMoveHandler::AgentMoveHandler() : EventHandler(std::list<std::string>({ "AgentMove" }))
{
}

void AgentMoveHandler::onEvent(std::shared_ptr<Event> e)
{
    if (e->getType() == "AgentMove") {
        AgentMoveEvent event = *std::static_pointer_cast<AgentMoveEvent>(e);

        sf::Vector2f distance = event.getDistance();
        Mobile* mobile = event.getObject();

        // If there is a right collision and the object is moving right, stop
        if (distance.x > 0 && mobile->getRightCollision())
        {
            distance.x = 0;
        }

        // If there is a left collision and the object is moving left, stop
        if (distance.x < 0 && mobile->getLeftCollision())
        {
            distance.x = 0;
        }

        // If there is a down collision and the object is moving down, stop
        if (distance.y > 0 && mobile->getDownCollision())
        {
            distance.y = 0;
        }

        // If there is an up collision and the object is moving up, stop
        if (distance.y < 0 && mobile->getUpCollision())
        {
            distance.y = 0;
        }

        event.getObject()->moveDistance(distance);
    }
}
