#pragma once
#include "PolyMath.h"
#include "Framework.h"

class GameObject {
public:
	Sprite* currentSprite;
	Vector2 scale;
	Vector2 position;
	BoundingBox boundingBox;
	GameObject();
	GameObject(Sprite* spr);
	void Scale(int x, int y);
	void SetScale(int x, int y);
	void Move(int x, int y);
	void SetPosition(int x, int y);
	bool IsColliding(GameObject otherObject);
};

class PhysicsObject {
	Vector2 currentVelocity;
public:
	virtual void Tick();
};
/*class Player : GameObject {

};
class Bullet : GameObject {

};*/

class Scene {
	GameObject objects[512] = {};
	PhysicsObject physObjects[32] = {};
public:
	Scene();
	void Draw();
	void PhysTick();
};