#pragma once
#include "EventHandler.h"
class AgentMoveHandler :
    public EventHandler
{
public:
    AgentMoveHandler();
    void onEvent(std::shared_ptr<Event> e) override;
};

