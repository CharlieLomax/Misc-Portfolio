#pragma once
#include "ScriptedEventHandler.h"
class SpawnHandler :
    public ScriptedEventHandler
{
public:
    SpawnHandler();
    void onEvent(std::shared_ptr<Event> e) override;
};

