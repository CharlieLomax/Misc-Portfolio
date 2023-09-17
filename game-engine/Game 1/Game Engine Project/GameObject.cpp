#include "GameObject.h"

std::list<GameObject*> GameObject::objects = std::list<GameObject*>();
std::mutex GameObject::listMutex;

void GameObject::moveObject(float delta_t)
{
	if (move != NULL) {
		move->move(this, delta_t);
	}
}

void GameObject::checkCollision(Mobile* other)
{
	if (collision != NULL) {
		collision->checkCollision(this, other);
	}
}

int GameObject::getID()
{
	std::lock_guard<std::mutex> lock(m);
	return GUID;
}

void GameObject::draw(sf::RenderWindow* window)
{
	std::lock_guard<std::mutex> lock(m);
	if (obj != NULL) {
		obj->draw(window);
	}
}

sf::FloatRect GameObject::getBox()
{
	std::lock_guard<std::mutex> lock(m);
	if (obj != NULL) {
		return obj->getBox();
	}

	return sf::FloatRect(0.f, 0.f, 0.f, 0.f);
}

sf::Vector2f GameObject::getVelocity()
{
	std::lock_guard<std::mutex> lock(m);
	return velocity;
}

void GameObject::setCollision(Collision* collision)
{
	std::lock_guard<std::mutex> lock(m);
	this->collision = collision;
}

void GameObject::setMove(Move* move)
{
	std::lock_guard<std::mutex> lock(m);
	this->move = move;
}

void GameObject::setObj(RenderObject* obj)
{
	std::lock_guard<std::mutex> lock(m);
	this->obj = obj;
}

void GameObject::setVelocity(sf::Vector2f velocity)
{
	std::lock_guard<std::mutex> lock(m);
	this->velocity = velocity;
}

void GameObject::updateVelocity(sf::Vector2f update)
{
	std::lock_guard<std::mutex> lock(m);
	velocity += update;
}

sf::Vector2f GameObject::getPosition()
{
	std::lock_guard<std::mutex> lock(m);
	if (obj != NULL) {
		return obj->getPosition();
	}

	return sf::Vector2f(0.f, 0.f);
}

sf::Color GameObject::getColor()
{
	std::lock_guard<std::mutex> lock(m);
	if (obj != NULL) {
		return obj->getColor();
	}

	return sf::Color();
}

sf::Vector2f GameObject::getSize()
{
	std::lock_guard<std::mutex> lock(m);
	if (obj != NULL) {
		return obj->getSize();
	}

	return sf::Vector2f(0.f, 0.f);
}

void GameObject::setPosition(sf::Vector2f position)
{
	std::lock_guard<std::mutex> lock(m);
	if (obj != NULL) {
		obj->setPosition(position);
	}
}

void GameObject::setPosition(float x, float y)
{
	std::lock_guard<std::mutex> lock(m);
	if (obj != NULL) {
		obj->setPosition(x, y);
	}
}

void GameObject::moveDistance(sf::Vector2f distance)
{
	std::lock_guard<std::mutex> lock(m);
	if (obj != NULL) {
		obj->moveDistance(distance);
	}
}

void GameObject::moveDistance(float x, float y)
{
	std::lock_guard<std::mutex> lock(m);
	if (obj != NULL) {
		obj->moveDistance(x, y);
	}
}

std::list<GameObject*> GameObject::getObjects()
{
	std::lock_guard<std::mutex> lock(listMutex);
	return objects;
}

GameObject::GameObject() {
	static int nextID = 0;
	GUID = nextID;
	nextID++;
	velocity = sf::Vector2f(0.f, 0.f);

	collision = NULL;
	move = NULL;
	obj = NULL;

	std::lock_guard<std::mutex> lock(listMutex);
	objects.emplace_back(this);
}

GameObject::GameObject(const GameObject& source) : GameObject(source.collision, source.move, source.obj, source.velocity)
{
	GUID = source.GUID;
}

GameObject::GameObject(const GameObject&& source) : GameObject(source.collision, source.move, source.obj, source.velocity)
{
	GUID = source.GUID;
}

GameObject::GameObject(Collision* collision, Move* move, RenderObject* obj) : GameObject() {
	setCollision(collision);
	setMove(move);
	setObj(obj);
}

GameObject::GameObject(Collision* collision, Move* move, RenderObject* obj, sf::Vector2f velocity) : GameObject(collision, move, obj) {
	setVelocity(velocity);
}

GameObject::~GameObject()
{
	std::lock_guard<std::mutex> lock(listMutex);
	objects.remove(this);
}

void GameObject::operator=(const GameObject& other)
{
	GUID = other.GUID;
	collision = other.collision;
	move = other.move;
	obj = other.obj;
	velocity = other.velocity;
}
