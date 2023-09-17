#include "ScrollHandler.h"

ScrollHandler::ScrollHandler() : EventHandler(std::list<std::string>({ "Scroll" }))
{
}

void ScrollHandler::onEvent(std::shared_ptr<Event> e)
{
	if (e->getType() == "Scroll") {
		ScrollEvent event = *std::static_pointer_cast<ScrollEvent>(e);
		RenderObject::moveCamera(event.getDistance(), 0.f);
	}
}
