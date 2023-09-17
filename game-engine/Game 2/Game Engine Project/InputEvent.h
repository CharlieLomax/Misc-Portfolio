#pragma once
#include "Event.h"
#include "Character.h"
class InputEvent :
    public Event
{
public:
    InputEvent(float timestamp, float relativePriority, float age, sf::Keyboard::Key key, bool release);
    sf::Keyboard::Key getKey();
    bool isRelease();
private:
    sf::Keyboard::Key key;
    bool release;
};

