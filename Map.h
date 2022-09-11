#pragma once
#include "Objects.h"
#include "Framework.h"

class Map {
private:
	Vector2 size;
	Block blocks[576];
	BoundingBox borders;
public:
	Map();
	Map(BoundingBox borders);
	void Spawn();
	void PhysTick();
};