#include "Character.h"
#include "EventManager.h"
#include "SpawnEvent.h"
#include "CharacterCollision.h"

Character::Character(const std::string& filename, sf::Vector2f spawn) : Mobile(filename, spawn, new CharacterCollision())
{
    setSpeed(0.3f);
}

Character::Character(const std::string& filename, float spawnX, float spawnY) : Character(filename, sf::Vector2f(spawnX, spawnY))
{
}

void Character::die()
{
    EventManager::instance()->raise(new SpawnEvent(globalTime.getTime(), 0.f, 0.f, this));
}
