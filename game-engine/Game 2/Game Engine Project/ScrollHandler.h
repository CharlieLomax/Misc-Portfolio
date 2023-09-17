#pragma once
#include "EventHandler.h"
#include "ScrollEvent.h"
class ScrollHandler :
    public EventHandler
{
public:
    ScrollHandler();
    void onEvent(std::shared_ptr<Event> e) override;
};

