#pragma once
#include "ScriptedEventHandler.h"
#include "ReplayViewer.h"
class InputHandler :
    public ScriptedEventHandler
{
public:
    InputHandler(Mobile* character, ReplayViewer* viewer);
    void onEvent(std::shared_ptr<Event> e) override;
private:
    Mobile* character;
    ReplayViewer* viewer;
};

extern void pauseTime();
extern void timeScale();