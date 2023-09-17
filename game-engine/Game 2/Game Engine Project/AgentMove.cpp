#include "AgentMove.h"
#include "EventManager.h"
#include "Timeline.h"
#include "AgentMoveEvent.h"

extern Timeline globalTime;

AgentMove::AgentMove()
{
}

void AgentMove::move(GameObject* object, float delta_t)
{
    Mobile* mobile = (Mobile*)object;
    sf::Vector2f velocity = mobile->getVelocity();

    if(velocity != sf::Vector2f(0, 0) && delta_t != 0)
        EventManager::instance()->raise(new AgentMoveEvent(globalTime.getTime(), 0.f, 0.f, mobile, delta_t * velocity));

    mobile->setVelocity(sf::Vector2f(0, 0));
    mobile->setMoved(false);
}
