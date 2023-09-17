#include "InputHandler.h"
#include "InputEvent.h"
#include "RecordStartEvent.h"
#include "RecordStopEvent.h"
#include <dukglue/dukglue.h>

void startRecordEvent(InputEvent* event, ReplayViewer* viewer) {
	EventManager::instance()->raise(new RecordStartEvent(globalTime.getTime(), 0.f, event->getAge() + 1));
}

void stopRecordEvent(InputEvent* event, ReplayViewer* viewer) {
	EventManager::instance()->raise(new RecordStopEvent(globalTime.getTime(), 0.f, event->getAge() + 1));
}

InputHandler::InputHandler(Mobile* character, ReplayViewer* viewer) : ScriptedEventHandler(std::list<std::string>({ "Input" }))
{
	this->character = character;
	this->viewer = viewer;

	dukglue_register_method(getScripter().getContext(), &InputEvent::isRelease, "isRelease");
	dukglue_register_method(getScripter().getContext(), &Mobile::startJump, "startJump");
	dukglue_register_method(getScripter().getContext(), &Mobile::setMovingLeft, "setMovingLeft");
	dukglue_register_method(getScripter().getContext(), &Mobile::setMovingRight, "setMovingRight");
	dukglue_register_function(getScripter().getContext(), &timeScale, "timeScale");
	dukglue_register_function(getScripter().getContext(), &pauseTime, "pause");
	dukglue_register_method(getScripter().getContext(), &ReplayViewer::isRecording, "isRecording");
	dukglue_register_method(getScripter().getContext(), &ReplayViewer::playRecording, "playRecording");
	dukglue_register_function(getScripter().getContext(), &startRecordEvent, "startRecord");
	dukglue_register_function(getScripter().getContext(), &stopRecordEvent, "stopRecord");
}

void InputHandler::onEvent(std::shared_ptr<Event> e)
{
	if (e->getType() == "Input") {
		InputEvent event = *std::static_pointer_cast<InputEvent>(e);

		switch (event.getKey()) {
			case(sf::Keyboard::Key::Space) :
				/*if (!event.isRelease()) {
					character->startJump();
				}*/
				getScripter().runScript("onSpaceInput", 2, &event, character);
				break;

			case(sf::Keyboard::Key::A):
				//character->setMovingLeft(!event.isRelease());
				getScripter().runScript("onAInput", 2, &event, character);
				break;

			case(sf::Keyboard::Key::D):
				//character->setMovingRight(!event.isRelease());());
				getScripter().runScript("onDInput", 2, &event, character);
				break;

			case(sf::Keyboard::Key::Tab):
				/*if (!event.isRelease()) {
					timeScale();
				}*/
				getScripter().runScript("onTabInput", 2, &event, character);
				break;

			case(sf::Keyboard::Key::Escape):
				/*if (!event.isRelease()) {
					pauseTime();
				}*/
				getScripter().runScript("onEscInput", 2, &event, character);
				break;

			case(sf::Keyboard::Key::R):
				/*if (!event.isRelease()) {
					if (viewer->isRecording()) {
						EventManager::instance()->raise(new RecordStopEvent(globalTime.getTime(), 0.f, event.getAge() + 1));
					}
					else {
						EventManager::instance()->raise(new RecordStartEvent(globalTime.getTime(), 0.f, event.getAge() + 1));
					}
				}*/
				getScripter().runScript("onRInput", 2, &event, viewer);
				break;

			case(sf::Keyboard::Key::Num1):
				/*if (!event.isRelease()) {
					viewer->playRecording(0.5);
				}*/
				getScripter().runScript("on1Input", 2, &event, viewer);
				break;

			case(sf::Keyboard::Key::Num2):
				/*if (!event.isRelease()) {
					viewer->playRecording(1);
				}*/
				getScripter().runScript("on2Input", 2, &event, viewer);
				break;

			case(sf::Keyboard::Key::Num3):
				/*if (!event.isRelease()) {
					viewer->playRecording(2);
				}*/
				getScripter().runScript("on3Input", 2, &event, viewer);
				break;
		}
	}
}
