#pragma once
#include "Move.h"
#include "Mobile.h"
class AgentMove :
    public Move
{
public:
    AgentMove();
    void move(GameObject* object, float delta_t) override;
};

