#pragma once
#include "Collision.h"
class SpawnCollision :
    public Collision
{
public:
    SpawnCollision(sf::FloatRect hitbox, sf::Vector2f point);
    void checkCollision(GameObject* object, Mobile* other) override;

private:
    sf::FloatRect hitbox;
    sf::Vector2f point;
};

