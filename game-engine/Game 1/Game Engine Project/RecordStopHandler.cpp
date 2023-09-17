#include "RecordStopHandler.h"
#include "RecordStopEvent.h"

RecordStopHandler::RecordStopHandler(ReplayViewer* viewer) : EventHandler(std::list<std::string>({ "RecordStop" }))
{
	this->viewer = viewer;
}

void RecordStopHandler::onEvent(std::shared_ptr<Event> e)
{
	if (e->getType() == "RecordStop") {
		viewer->stopRecording();
	}
}
