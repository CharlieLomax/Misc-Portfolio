#include "Mobile.h"
#include "AgentMove.h"
#include "EventManager.h"
#include "SpawnEvent.h"

Mobile::Mobile() : GameObject(NULL, new AgentMove(), NULL)
{
    leftCollision = false;
    rightCollision = false;
    upCollision = false;
    downCollision = false;

    movingRight = false;
    movingLeft = false;

    moved = false;

    jumpStart = 0;
    jump = false;

    speed = 0;
}

Mobile::Mobile(const std::string& filename, sf::Vector2f spawn) : Mobile() {
    setObj(new RenderSprite(filename));
    
    setSpawn(spawn);
    setPosition(spawn);
}

Mobile::Mobile(const Mobile& other) : GameObject(other)
{
    leftCollision = other.leftCollision;
    rightCollision = other.rightCollision;
    upCollision = other.upCollision;
    downCollision = other.downCollision;

    movingRight = other.movingRight;
    movingLeft = other.movingLeft;

    moved = other.moved;

    jumpStart = other.jumpStart;
    jump = other.jump;

    speed = other.speed;

    spawn = other.spawn;
}

Mobile::Mobile(const Mobile&& other) : GameObject(other)
{
    leftCollision = other.leftCollision;
    rightCollision = other.rightCollision;
    upCollision = other.upCollision;
    downCollision = other.downCollision;

    movingRight = other.movingRight;
    movingLeft = other.movingLeft;

    moved = other.moved;

    jumpStart = other.jumpStart;
    jump = other.jump;

    speed = other.speed;

    spawn = other.spawn;
}

void Mobile::operator=(const Mobile& other)
{
    GameObject::operator=(other);

    speed = other.speed;
    spawn = other.spawn;
}

void Mobile::fall() {
    // Move down due to gravity
    updateVelocity(sf::Vector2f(0.f, 0.2f));
}

void Mobile::die()
{
    EventManager::instance()->raise(new SpawnEvent(globalTime.getTime(), 0.f, 0.f, this));
}

void Mobile::respawn()
{
    setPosition(spawn);
}

sf::Vector2f Mobile::getSpawn()
{
    std::lock_guard<std::mutex> lock(m);
    return spawn;
}

void Mobile::setSpawn(sf::Vector2f spawn)
{
    std::lock_guard<std::mutex> lock(m);
    this->spawn = spawn;
}

void Mobile::clearCollision() {
    std::lock_guard<std::mutex> lock(m);
    leftCollision = false;
    rightCollision = false;
    upCollision = false;
    downCollision = false;
}

bool Mobile::getLeftCollision() {
    std::lock_guard<std::mutex> lock(m);
    return leftCollision;
}

bool Mobile::getRightCollision() {
    std::lock_guard<std::mutex> lock(m);
    return rightCollision;
}

bool Mobile::getUpCollision() {
    std::lock_guard<std::mutex> lock(m);
    return upCollision;
}

bool Mobile::getDownCollision() {
    std::lock_guard<std::mutex> lock(m);
    return downCollision;
}

void Mobile::collideLeft()
{
    std::lock_guard<std::mutex> lock(m);
    leftCollision = true;
}

void Mobile::collideRight()
{
    std::lock_guard<std::mutex> lock(m);
    rightCollision = true;
}

void Mobile::collideUp()
{
    std::lock_guard<std::mutex> lock(m);
    upCollision = true;
}

void Mobile::collideDown()
{
    std::lock_guard<std::mutex> lock(m);
    downCollision = true;
}

void Mobile::setMovingRight(bool movingRight)
{
    this->movingRight = movingRight;
}

void Mobile::setMovingLeft(bool movingLeft)
{
    this->movingLeft = movingLeft;
}

void Mobile::setMoved(bool moved)
{
    this->moved = moved;
}

void Mobile::inputVelocity()
{
    // Move right when a right move is active
    if (movingRight && !moved)
    {
        updateVelocity(sf::Vector2f(speed, 0.f));
    }

    // Move left when a left move is active
    if (movingLeft && !moved)
    {
        updateVelocity(sf::Vector2f(-speed, 0.f));
    }

    moved = true;
}

void Mobile::startJump()
{
    // When space is pressed and the character is standing on something, a jump is activated. This will cause them to move up for the next 500 tics.
    if (getDownCollision()) {
        jumpStart = globalTime.getTime();
        jump = true;
    }
}

void Mobile::jumpVelocity()
{
    // If a jump is active, move the character up. if the jump is over, 
    if (globalTime.getTime() - jumpStart < 500 && jump) {
        updateVelocity(sf::Vector2f(0.f, -0.5f));
    }
    else if (jump) {
        jump = false;
    }
}

float Mobile::getSpeed()
{
    return speed;
}

void Mobile::setSpeed(float speed)
{
    this->speed = speed;
}
