#pragma once
#include "Event.h"
class EventHandler
{
public:
	virtual void onEvent(std::shared_ptr<Event> e) = 0;

	EventHandler(std::list<std::string> types);
	~EventHandler();

private:
	std::list<std::string> types;
};

