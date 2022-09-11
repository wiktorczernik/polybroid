#pragma once
#include "Objects.h"
#include "Framework.h"
#include <iostream>

using namespace std;

class Map {
private:
	Vector2 size;
	list<Block> blocks;
	list<Bullet> bullets;
	BoundingBox borders;
	list<GameObject> toDraw;
public:
	Map();
	Map(BoundingBox borders);
	void Spawn();
	void Tick();
	std::list<GameObject>& ObjectsToDraw();
};