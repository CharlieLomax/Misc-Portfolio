#pragma once
#include "MobileCollision.h"
class CharacterCollision :
    public MobileCollision
{
public:
    CharacterCollision();

    void clearCollision() override;

    bool getLeftCollision() override;
    bool getRightCollision() override;
    bool getUpCollision() override;
    bool getDownCollision() override;

    void collideLeft() override;
    void collideRight() override;
    void collideUp() override;
    void collideDown() override;

private:
    bool leftCollision;
    bool rightCollision;
    bool upCollision;
    bool downCollision;
};

