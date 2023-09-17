#include "ConnectHandler.h"
#include "ConnectEvent.h"
#include "OtherCharacter.h"

ConnectHandler::ConnectHandler(const std::string& filename) : EventHandler(std::list<std::string>({ "Connect" }))
{
	this->filename = filename;
}

void ConnectHandler::onEvent(std::shared_ptr<Event> e)
{
	if (e->getType() == "Connect") {
		ConnectEvent event = *std::static_pointer_cast<ConnectEvent>(e);

		OtherCharacter::connect(event.getId(), filename);
	}
}
