#include "DisconnectHandler.h"
#include "OtherCharacter.h"
#include "DisconnectEvent.h"

DisconnectHandler::DisconnectHandler() : EventHandler(std::list<std::string>({ "Disconnect" }))
{
}

void DisconnectHandler::onEvent(std::shared_ptr<Event> e)
{
	if (e->getType() == "Disconnect") {
		DisconnectEvent event = *std::static_pointer_cast<DisconnectEvent>(e);

		OtherCharacter::disconnect(event.getId());
	}
}
