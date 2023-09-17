#pragma once
#include "EventHandler.h"
#include "ScriptManager.h"
class ScriptedEventHandler :
    public EventHandler
{
public:
    ScriptedEventHandler(std::list<std::string> types);
    ScriptManager& getScripter();

private:
    ScriptManager scripter;
};

