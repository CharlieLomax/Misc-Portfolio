#include "CharacterCollision.h"

CharacterCollision::CharacterCollision()
{
    leftCollision = false;
    rightCollision = false;
    upCollision = false;
    downCollision = false;
}

void CharacterCollision::clearCollision() {
    leftCollision = false;
    rightCollision = false;
    upCollision = false;
    downCollision = false;
}

bool CharacterCollision::getLeftCollision() {
    return leftCollision;
}

bool CharacterCollision::getRightCollision() {
    return rightCollision;
}

bool CharacterCollision::getUpCollision() {
    return upCollision;
}

bool CharacterCollision::getDownCollision() {
    return downCollision;
}

void CharacterCollision::collideLeft()
{
    leftCollision = true;
}

void CharacterCollision::collideRight()
{
    rightCollision = true;
}

void CharacterCollision::collideUp()
{
    upCollision = true;
}

void CharacterCollision::collideDown()
{
    downCollision = true;
}