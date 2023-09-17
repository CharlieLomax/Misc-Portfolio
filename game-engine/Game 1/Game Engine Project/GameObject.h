#pragma once
#include "Collision.h"
#include "Move.h"
#include <mutex>
#include "RenderObject.h"
#include <list>

class GameObject
{
public:
	std::mutex m;
	GameObject();
	GameObject(const GameObject&);
	GameObject(const GameObject&&);
	GameObject(Collision* collision, Move* move, RenderObject* obj);
	GameObject(Collision* collision, Move* move, RenderObject* obj, sf::Vector2f velocity);
	~GameObject();

	void operator=(const GameObject&);
	
	void moveObject(float delta_t);
	void checkCollision(Mobile* other);
	
	int getID();
	void draw(sf::RenderWindow* window);
	sf::FloatRect getBox();
	sf::Vector2f getVelocity();
	
	void setCollision(Collision* collision);
	void setMove(Move* move);
	void setObj(RenderObject* obj);

	void setVelocity(sf::Vector2f velocity);
	/* Updates velocity by adding the given update value to it */
	void updateVelocity(sf::Vector2f update);

	sf::Vector2f getPosition();
	sf::Color getColor();
	sf::Vector2f getSize();
	void setPosition(sf::Vector2f position);
	void setPosition(float x, float y);
	void moveDistance(sf::Vector2f distance);
	void moveDistance(float x, float y);

	static std::mutex listMutex;
	static std::list<GameObject*> getObjects();

private:
	int GUID;
	Collision* collision;
	Move* move;
	RenderObject* obj;
	sf::Vector2f velocity;

	static std::list<GameObject*> objects;
};

