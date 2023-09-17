#pragma once
#include "Move.h"
class BasicMove :
    public Move
{
public:
    BasicMove();
    void move(GameObject* object, float delta_t) override;
};

