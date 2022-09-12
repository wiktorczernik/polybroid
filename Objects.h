#pragma once
#include "Objects.h"
#include "Framework.h"
#include "PolyMath.h"
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

class Timer {
private:
	unsigned int currentTime;
	unsigned int triggerTime;
public:
	bool* trigger;
	bool autoRestart;
	bool triggered;
	Timer();
	void Setup(bool* Trigger, unsigned int TriggerTime, bool AutoRestart);
	void Tick(int deltaTime);
	void Restart();
};

#pragma region Objects
class Object {
public:
	Vector2 scale;
	Vector2 position;
	void Scale(int x, int y);
	void SetScale(int x, int y);
	void Move(int x, int y);
	void SetPosition(int x, int y);
	Object() {

	}
	Object(Vector2 Position, Vector2 Scale) {
		position = Position;
		scale = Scale;
	}
};

class VisualObject : public Object {
public:
	Sprite* sprite;
	VisualObject() : Object() {

	}
	VisualObject(Vector2 Position, Vector2 Scale, Sprite* Sprite) : Object(Position, Scale) {
		position = Position;
		scale = Scale;
		sprite = Sprite;
	}
};
class GameObject : public Object {
private:
	void UpdateBoundingBox();
public:
	Sprite* currentSprite;
	Sprite* idleSprite;
	BoundingBox boundingBox;
	bool IsAlive;
	int health;
	void Scale(int x, int y);
	void SetScale(int x, int y);
	void Move(int x, int y);
	void SetPosition(int x, int y);
	bool Hurt();
	GameObject() : Object() {
		health = -1;
		IsAlive = true;
	}
	GameObject(Vector2 Position, Vector2 Scale, Sprite* IdleSprite) : Object(Position, Scale) {
		health = -1;
		IsAlive = true;
		idleSprite = IdleSprite;
		currentSprite = idleSprite;
		position = Position;
		scale = Scale;
		UpdateBoundingBox();

	}
	bool IsColliding(GameObject otherObject);
};
class Block : public GameObject {
private:
	int id;
	Sprite* brokenSprite;
public:
	bool Hurt();
	Block() : GameObject() {
		IsAlive = true;
		health = -1;
	}
	Block(Vector2 Position, Vector2 Scale, Sprite* IdleSprite, Sprite* BrokenSprite, int Id, int Health) : GameObject(Position, Scale, IdleSprite) {
		IsAlive = true;
		health = Health;
		idleSprite = IdleSprite;
		brokenSprite = BrokenSprite;
		currentSprite = idleSprite;
		id = Id;
	}
};
class PhysicsObject : public GameObject {
protected:
	Vector2 initVelocity;
	BoundingBox canvas;
public:
	PhysicsObject() : GameObject() {
		IsAlive = true;
		health = -1;
	}
	PhysicsObject(BoundingBox Canvas, Vector2 Position, Vector2 Scale, Vector2 Velocity, Sprite* IdleSprite) : GameObject(Position, Scale, IdleSprite) {
		IsAlive = true;
		health = -1;
		canvas = Canvas;
		position = Position;
		scale = Scale;
		initVelocity = Velocity;
		currentVelocity = Velocity;
		idleSprite = IdleSprite;
		currentSprite = idleSprite;
	}
	Vector2 currentVelocity;
	bool CollidesBorder(bool* result) {
		bool answer = false;
		if (boundingBox.a.x <= canvas.a.x || boundingBox.b.x >= canvas.b.x) {
			result[0] = true;
			answer = true;
		}
		if (boundingBox.a.y <= canvas.b.y || boundingBox.c.y >= canvas.d.y) {
			result[1] = true;
			answer = true;
		}
		return answer;
	}
	bool CollidesWith(GameObject otherObject) {
		BoundingBox a = boundingBox;
		BoundingBox b = otherObject.boundingBox;

		return a.Intersects(b);
	}
	void InvertVelocity(bool X, bool Y) {
		currentVelocity.x *= (X == true ? -1 : 1);
		currentVelocity.y *= (Y == true ? -1 : 1);
	}
	void Tick();
};
#pragma endregion

class Player : public PhysicsObject {
private:
	int abilityMultiplier;

	bool decreasePositiveAB;
	bool decreaseNegativeAB;

	list<Timer> timers;

	int moveInput;
public:
	bool catchedBullet;
	Player() : PhysicsObject() {

	}
	Player(BoundingBox Canvas, Vector2 Position, Vector2 Scale, Vector2 Velocity, Sprite* IdleSprite) : PhysicsObject(Canvas, Position, Scale, Velocity, IdleSprite) {
		IsAlive = true;
		health = -1;
		canvas = Canvas;
		position = Position;
		scale = Scale;
		initVelocity = Velocity;
		currentVelocity = Velocity;
		idleSprite = IdleSprite;
		currentSprite = idleSprite;
		abilityMultiplier = 0;

		decreasePositiveAB = false;
		decreaseNegativeAB = false;
	}
	void UpdateInput(int Move) {
		moveInput = Move;
	}
	void AddAbility(bool IsPositive);
	void Tick(int deltaTime);
};
class Bullet : public PhysicsObject {
public:
	Bullet() : PhysicsObject() {
		IsAlive = true;
		health = -1;
	}
	Bullet(BoundingBox Canvas, Vector2 Position, Vector2 Scale, Vector2 Velocity, Vector2 DefaultVelocity, Sprite* Sprite) : PhysicsObject(Canvas, Position, Scale, Velocity, Sprite) {
		IsAlive = true;
		health = -1;
		canvas = Canvas;
		canvas.c.y += Scale.y * 3;
		canvas.d.y += Scale.y * 3;
		position = Position;
		scale = Scale;
		initVelocity = DefaultVelocity;
		currentVelocity = Velocity;
		idleSprite = Sprite;
		currentSprite = idleSprite;
	}
	void Tick(list<Block>& objects, Player& player);
};
class Ability : public PhysicsObject {
public:
	bool isPositive;
	Ability(BoundingBox Canvas, Vector2 Position, Vector2 Scale, Vector2 Velocity, bool IsPositive, Sprite* Sprite) : PhysicsObject(Canvas, Position, Scale, Velocity, Sprite) {
		IsAlive = true;
		health = -1;
		canvas = Canvas;

		canvas.a.y -= Scale.y * 4;
		canvas.b.y = canvas.a.y;
		canvas.c.y += Scale.y * 4;
		canvas.d.y = canvas.c.y;

		position = Position;
		scale = Scale;
		currentVelocity = Velocity;
		isPositive = IsPositive;

	}
	void Tick(Player& player);
};