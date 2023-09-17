#pragma once
#include "Move.h"
class PlatformMove :
    public Move
{
public:
    PlatformMove(float moveSpeed, std::vector<sf::Vector2f> turnsVector);
    void move(GameObject* object, float delta_t) override;

private:
    float speed;
    std::vector<sf::Vector2f> turns;
    std::vector<sf::Vector2f>::iterator nextTurn;

    /* Returns the unit vector of the given sf::Vector2f object. Used in velocity calculations. */
    sf::Vector2f hat(sf::Vector2f);
};

