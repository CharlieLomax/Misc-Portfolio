#pragma once
#include "EventHandler.h"
#include "ReplayViewer.h"
class RecordStopHandler :
    public EventHandler
{
public:
    RecordStopHandler(ReplayViewer* viewer);
    void onEvent(std::shared_ptr<Event> e) override;
private:
    ReplayViewer* viewer;
};

