#pragma once
#include "EventHandler.h"
#include "MoveEvent.h"
class MoveHandler :
    public EventHandler
{
public:
    MoveHandler();
    void onEvent(std::shared_ptr<Event> e) override;
};

