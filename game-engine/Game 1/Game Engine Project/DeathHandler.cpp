#include "DeathHandler.h"
#include <dukglue/dukglue.h>

DeathHandler::DeathHandler() : ScriptedEventHandler(std::list<std::string>({ "Death" }))
{
	dukglue_register_method(getScripter().getContext(), &DeathEvent::getObject, "getObj");
	dukglue_register_method(getScripter().getContext(), &Mobile::die, "die");
}

void DeathHandler::onEvent(std::shared_ptr<Event> e)
{
	if (e->getType() == "Death") {
		DeathEvent event = *std::static_pointer_cast<DeathEvent>(e);
		getScripter().runScript("onDeath", 1, &event);
	}
}
