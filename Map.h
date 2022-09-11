#pragma once
#include "Objects.h"
#include "Framework.h"
#include <iostream>
#include "Managers.h"

using namespace std;

class Map {
private:
	AssetManager assetManager;
	BoundingBox border;
	list<Block> blocks;
	list<Bullet> bullets;
	BoundingBox borders;
	list<GameObject> toDraw;
	void RemoveDeadmen();
public:
	Map(BoundingBox Border, AssetManager& AssetManager);
	Map();
	void Startup();
	void Tick();
	std::list<GameObject>& ObjectsToDraw();
};