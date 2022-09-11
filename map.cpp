#include "Map.h"
#include "Objects.h"
#include "Managers.h"
#include <iostream>


Map::Map(BoundingBox Border, AssetManager& AssetManager) {

}
Map::Map()
{
	blocks = list<Block>();
	bullets = list<Bullet>();
	toDraw = list<GameObject>();
};

void Map::Startup() {
	Block a = Block();
	toDraw.push_back(a);
}
void Map::Tick() {
	RemoveDeadmen();
}
list<GameObject>& Map::ObjectsToDraw() {
	toDraw.clear();
	return toDraw;
}
void Map::RemoveDeadmen() {

}