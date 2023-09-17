#pragma once
#include "EventHandler.h"
#include "ReplayViewer.h"
class RecordStartHandler :
    public EventHandler
{
public:
    RecordStartHandler(ReplayViewer* viewer);
    void onEvent(std::shared_ptr<Event> e) override;
private:
    ReplayViewer* viewer;
};

