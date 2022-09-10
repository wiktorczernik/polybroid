#pragma once
#include "Objects.h"
#include "Framework.h"
#include "PolyMath.h"
#include <string>

#pragma region Assets
enum AssetType {
	AssetGameObject,
	AssetBlock,
	AssetMap
};

class Asset {
public:
	std::string path;
};
class GameObjectAsset : public Asset {
public:
	std::string currentSprite;
	std::string idleSprite;
};
class BlockAsset : public GameObjectAsset {
public:
	std::string brokenSprite;
	int id;
	int health;
	BlockAsset();
};
#pragma endregion
#pragma region Objects
class GameObject {
public:
	Sprite* currentSprite;
	Sprite* idleSprite;
	Vector2 scale;
	Vector2 position;
	BoundingBox boundingBox;
	GameObject(Sprite* IdleSprite);
	GameObject();
	void Scale(int x, int y);
	void SetScale(int x, int y);
	void Move(int x, int y);
	void SetPosition(int x, int y);
	bool IsColliding(GameObject otherObject);
};
class Block : public GameObject {
private:
	int id;
	int health;
	Sprite* brokenSprite;
public:
	void Hurt();
	Block(int Id, int Health, Sprite* IdleSprite, Sprite* BrokenSprite);
};
class PhysicsObject : public GameObject {
	Vector2 currentVelocity;
public:
	virtual void Tick();
};
#pragma endregion