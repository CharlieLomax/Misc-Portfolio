#include "DrawHandler.h"
#include "DrawEvent.h"

DrawHandler::DrawHandler(sf::RenderWindow* window) : EventHandler(std::list<std::string>({ "Draw" }))
{
	this->window = window;
}

void DrawHandler::onEvent(std::shared_ptr<Event> e)
{
	if (e->getType() == "Draw") {
		DrawEvent event = *std::static_pointer_cast<DrawEvent>(e);

        // Clear out everything already rendered
        window->clear(sf::Color::Blue);

        // Draw the objects that should appear onscreen
        for (GameObject* object : GameObject::getObjects()) {
            object->draw(window);
        }

        window->display();
	}
}
