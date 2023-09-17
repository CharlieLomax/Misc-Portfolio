#include "ReplayViewer.h"

ReplayViewer::ReplayViewer(sf::RenderWindow* window) : EventHandler(std::list<std::string>({ "Connect", "Disconnect", "Reposition", "Move", "AgentMove", "Spawn", "Scroll", "Collision", "Draw" }))
{
	recording = false;
	this->window = window;
}

void ReplayViewer::startRecording()
{
	for (GameObject* obj : GameObject::getObjects()) {
		startPositions[obj->getID()] = obj->getPosition();
	}
	
	recording = true;
}

void ReplayViewer::stopRecording()
{
	recording = false;
}

void ReplayViewer::playRecording(float speed)
{
	if (!events.empty()) {
		recording = false;

		std::unordered_map<std::string, std::list<EventHandler*>*> handlers = EventManager::instance()->getHandlers();

		// Save the position of each game object
		for (GameObject* obj : GameObject::getObjects()) {
			endPositions[obj->getID()] = obj->getPosition();
			obj->setPosition(startPositions[obj->getID()]);
		}

		float startTimestamp = events.front()->getTimestamp();
		Timeline replayTime(&globalTime, speed);

		bool tabPressed = false;

		while (!events.empty()) {
			if (!tabPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab)) {
				tabPressed = true;
				replayTime.pause();
				if (replayTime.getTic() == 1) {
					replayTime.setTic(2);
				}
				else if (replayTime.getTic() == 2) {
					replayTime.setTic(0.5);
				}
				else if (replayTime.getTic() == 0.5) {
					replayTime.setTic(1);
				}
				replayTime.unpause();
			}

			else if (tabPressed && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab)) {
				tabPressed = false;
			}
			
			std::shared_ptr<Event> e = events.front();

			// Wait until an appropriate amount of time has passed since the last event
			float delta_t = replayTime.getTime();
			float delta_timestamp = events.front()->getTimestamp() - startTimestamp;
			while (delta_t < delta_timestamp) {
				delta_t = replayTime.getTime();
			}

			if (handlers.count(e->getType()) > 0) {
				for (EventHandler* handler : *handlers.at(e->getType())) {
					handler->onEvent(e);
				}
			}

			events.pop_front();

		}

		for (GameObject* obj : GameObject::getObjects()) {
			obj->setPosition(endPositions[obj->getID()]);
		}
	}
}

void ReplayViewer::onEvent(std::shared_ptr<Event> e)
{
	if (recording) {
		events.push_back(e);
	}
}

bool ReplayViewer::isRecording()
{
	return recording;
}
