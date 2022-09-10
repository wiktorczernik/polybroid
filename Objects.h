#pragma once
#include "Objects.h"
#include "Framework.h"
#include "PolyMath.h"
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

#pragma region Assets
enum AssetType {
	AssetGameObject,
	AssetBlock,
	AssetMap
};

class Asset {
public:
	fs::path path;
};
class GameObjectAsset : public Asset {
public:
	fs::path idleSprite;
};
class BlockAsset : public GameObjectAsset {
public:
	fs::path brokenSprite;
	int id;
	int health;
	BlockAsset();
};
#pragma endregion
#pragma region Objects
class GameObject {
private:
	void UpdateBoundingBox();
public:
	Sprite* currentSprite;
	Sprite* idleSprite;
	Vector2 scale;
	Vector2 position;
	BoundingBox boundingBox;
	GameObject(Vector2 Position, Vector2 Scale, Sprite* IdleSprite) {
		idleSprite = IdleSprite;
		currentSprite = idleSprite;
		position = Position;
		scale = Scale;
		UpdateBoundingBox();

	}
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
	Block(Vector2 Position, Vector2 Scale, Sprite* IdleSprite, Sprite* BrokenSprite, int Id, int Health) : GameObject(Position, Scale, IdleSprite) {
		brokenSprite = BrokenSprite;
		id = Id;
		health = Health;
	}
};
class PhysicsObject : public GameObject {
	Vector2 currentVelocity;
public:
	virtual void Tick();
};
#pragma endregion