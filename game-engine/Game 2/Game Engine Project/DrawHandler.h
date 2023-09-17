#pragma once
#include "EventHandler.h"
class DrawHandler :
    public EventHandler
{
public:
    DrawHandler(sf::RenderWindow* window);
    void onEvent(std::shared_ptr<Event> e) override;
private:
    sf::RenderWindow* window;
};

