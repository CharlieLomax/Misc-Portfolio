#pragma once
#include <list>
#include "EventHandler.h"
#include <queue>
#include <unordered_map>
class EventManager
{
public:
	void registerHandler(std::list<std::string> types, EventHandler* handler);
	void deregisterHandler(std::list<std::string> types, EventHandler* handler);
	void raise(Event* e);
	void raise(std::shared_ptr<Event> e);

	void setPriority(float priority);

	static EventManager* instance();

	void manage();

	std::unordered_map<std::string, std::list<EventHandler*>*> getHandlers();

	std::recursive_mutex m;

private:
	EventManager();

	std::unordered_map<std::string, std::list<EventHandler*>*> handlers;
	std::priority_queue<std::shared_ptr<Event>> raisedEvents;
	float currentPriority;
};

