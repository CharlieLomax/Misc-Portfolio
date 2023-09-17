#pragma once
#include "EventHandler.h"

class RepositionHandler :
	public EventHandler
{
public:
	RepositionHandler();
	void onEvent(std::shared_ptr<Event> e) override;
};

