#include "SpawnHandler.h"
#include "SpawnEvent.h"
#include <dukglue/dukglue.h>

SpawnHandler::SpawnHandler() : ScriptedEventHandler(std::list<std::string>({ "Spawn" }))
{
	dukglue_register_method(getScripter().getContext(), &SpawnEvent::getObj, "getObj");
	dukglue_register_method(getScripter().getContext(), &Mobile::respawn, "respawn");
}

void SpawnHandler::onEvent(std::shared_ptr<Event> e)
{
	if (e->getType() == "Spawn") {
		SpawnEvent event = *std::static_pointer_cast<SpawnEvent>(e);
		getScripter().runScript("onSpawn", 1, &event);
	}
}
