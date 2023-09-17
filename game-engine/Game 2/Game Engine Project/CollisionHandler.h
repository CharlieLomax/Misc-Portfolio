#pragma once
#include "EventHandler.h"
#include "CollisionEvent.h"
class CollisionHandler :
    public EventHandler
{
public:
    CollisionHandler();
    void onEvent(std::shared_ptr<Event> e) override;
};

