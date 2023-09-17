#include "EventManager.h"

EventManager::EventManager() {
	handlers = std::unordered_map<std::string, std::list<EventHandler*>*>();
	raisedEvents = std::priority_queue<std::shared_ptr<Event>>();
	currentPriority = 0;
}

void EventManager::registerHandler(std::list<std::string> types, EventHandler* handler)
{
	std::lock_guard<std::recursive_mutex> lock(m);
	
	for (std::string type : types) {
		if (handlers.count(type) > 0) {
			handlers.at(type)->push_back(handler);
		}

		else {
			handlers.emplace(type, new std::list<EventHandler*>({ handler }));
		}
	}
}

void EventManager::deregisterHandler(std::list<std::string> types, EventHandler* handler)
{
	std::lock_guard<std::recursive_mutex> lock(m);

	for (std::string type : types) {
		handlers.at(type)->remove(handler);
	}
}

void EventManager::raise(Event* e)
{
	std::lock_guard<std::recursive_mutex> lock(m);
	raisedEvents.push(std::shared_ptr<Event>(e));
}

void EventManager::raise(std::shared_ptr<Event> e)
{
	std::lock_guard<std::recursive_mutex> lock(m);
	raisedEvents.push(e);
}

void EventManager::setPriority(float priority)
{
	std::lock_guard<std::recursive_mutex> lock(m);
	currentPriority = priority;
}

EventManager* EventManager::instance()
{
	static EventManager* instance = new EventManager();
	return instance;
}

void EventManager::manage()
{
	std::lock_guard<std::recursive_mutex> lock(m);
	
	while (!raisedEvents.empty()) {
		std::shared_ptr<Event> e = raisedEvents.top();
		raisedEvents.pop();

		if (currentPriority == 0 || e->getPriority() <= currentPriority) {
			
			if (handlers.count(e->getType()) > 0) {
				for (EventHandler* handler : *handlers.at(e->getType())) {
					handler->onEvent(e);
				}
			}

		}

		else {
			break;
		}
	}
}

std::unordered_map<std::string, std::list<EventHandler*>*> EventManager::getHandlers()
{
	std::lock_guard<std::recursive_mutex> lock(m);
	return handlers;
}
