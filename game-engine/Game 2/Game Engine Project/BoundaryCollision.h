#pragma once
#include "Collision.h"
class BoundaryCollision :
    public Collision
{
public:
    BoundaryCollision(float posLeft, float posright);
    void checkCollision(GameObject* object, Mobile* other) override;

private:
    float posLeft;
    float posRight;
};

