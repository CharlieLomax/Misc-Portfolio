#include "MovingCollision.h"
#include "Mobile.h"
#include "CollisionEvent.h"
#include "EventManager.h"

void MovingCollision::checkCollision(GameObject* object, Mobile* other) {
    // Get the hitboxes of both objects
    sf::FloatRect mobBox = other->getBox();
    sf::FloatRect platBox = object->getBox();

    if (platBox.top < mobBox.top + mobBox.height && platBox.top + platBox.height > mobBox.top) {
        // If the right edge of the platform hitbox is inside of the character's hitbox, the left edge is outside, and they are vertically aligned, there is a collision on the left side
        if (platBox.left + platBox.width >= mobBox.left && platBox.left < mobBox.left && platBox.left + platBox.width < mobBox.left + mobBox.width) {
            EventManager::instance()->raise(new CollisionEvent(globalTime.getTime(), 0.f, 0.f, CollisionEvent::Direction::LEFT, other, object));
        }

        // If the left edge of the platform hitbox is inside of the character's hitbox, the right edge is outside, and they are vertically aligned, there is a collision on the right side
        if (platBox.left <= mobBox.left + mobBox.width && platBox.left > mobBox.left && platBox.left + platBox.width > mobBox.left + mobBox.width) {
            EventManager::instance()->raise(new CollisionEvent(globalTime.getTime(), 0.f, 0.f, CollisionEvent::Direction::RIGHT, other, object));
        }
    }

    if (platBox.left + platBox.width > mobBox.left && platBox.left < mobBox.left + mobBox.width) {
        // If the bottom edge of the platform hitbox is inside of the character's hitbox, the top edge is outside, and they are horizontally aligned, there is a collision on the top
        if (platBox.top + platBox.height >= mobBox.top && platBox.top < mobBox.top && platBox.top + platBox.height < mobBox.top + mobBox.height) {
            EventManager::instance()->raise(new CollisionEvent(globalTime.getTime(), 0.f, 0.f, CollisionEvent::Direction::UP, other, object));
        }

        // If the top edge of the platform hitbox is inside of the character's hitbox, the bottom edge is outside, and they are horizontally aligned, there is a collision on the bottom
        if (platBox.top <= mobBox.top + mobBox.height && platBox.top > mobBox.top && platBox.top + platBox.height > mobBox.top + mobBox.height) {
            EventManager::instance()->raise(new CollisionEvent(globalTime.getTime(), 0.f, 0.f, CollisionEvent::Direction::DOWN, other, object));
            other->updateVelocity(object->getVelocity());
        }
    }
    
}