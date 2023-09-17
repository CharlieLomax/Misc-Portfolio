#include "ScriptedEventHandler.h"

ScriptedEventHandler::ScriptedEventHandler(std::list<std::string> types) : EventHandler(types)
{
	scripter = ScriptManager();

	scripter.loadScript("scriptFile.js");
}

ScriptManager& ScriptedEventHandler::getScripter()
{
	return scripter;
}
