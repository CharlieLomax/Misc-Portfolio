#include "EventHandler.h"
#include "EventManager.h"

EventHandler::EventHandler(std::list<std::string> types)
{
	this->types = types;
	EventManager::instance()->registerHandler(types, this);
}

EventHandler::~EventHandler()
{
	EventManager::instance()->deregisterHandler(types, this);
}
