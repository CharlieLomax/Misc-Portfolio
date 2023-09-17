#include "MoveHandler.h"

MoveHandler::MoveHandler() : EventHandler(std::list<std::string>({"Move"}))
{
}

void MoveHandler::onEvent(std::shared_ptr<Event> e)
{
	if (e->getType() == "Move") {
		MoveEvent event = *std::static_pointer_cast<MoveEvent>(e);
		event.getObject()->moveDistance(event.getDistance());
	}
}
