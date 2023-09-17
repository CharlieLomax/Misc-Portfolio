#include "RepositionHandler.h"
#include "RepositionEvent.h"

RepositionHandler::RepositionHandler() : EventHandler(std::list<std::string>({ "Reposition" }))
{
}

void RepositionHandler::onEvent(std::shared_ptr<Event> e)
{
	if (e->getType() == "Reposition") {
		RepositionEvent event = *std::static_pointer_cast<RepositionEvent>(e);
		event.getObject()->setPosition(event.getPos());
	}
}
