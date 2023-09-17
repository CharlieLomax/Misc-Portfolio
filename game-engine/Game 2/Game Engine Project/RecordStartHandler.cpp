#include "RecordStartHandler.h"
#include "RecordStartEvent.h"

RecordStartHandler::RecordStartHandler(ReplayViewer* viewer) : EventHandler(std::list<std::string>({ "RecordStart" }))
{
	this->viewer = viewer;
}

void RecordStartHandler::onEvent(std::shared_ptr<Event> e)
{
	if (e->getType() == "RecordStart") {
		viewer->startRecording();
	}
}
