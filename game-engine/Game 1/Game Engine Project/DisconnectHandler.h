#pragma once
#include "EventHandler.h"
class DisconnectHandler :
    public EventHandler
{
public:
	DisconnectHandler();
	void onEvent(std::shared_ptr<Event> e) override;
};

