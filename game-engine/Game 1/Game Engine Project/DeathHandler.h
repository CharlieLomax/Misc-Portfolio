#pragma once
#include "ScriptedEventHandler.h"
#include "DeathEvent.h"

class DeathHandler :
	public ScriptedEventHandler
{
public:
	DeathHandler();
	void onEvent(std::shared_ptr<Event> e) override;
};

