#include "Map.h"
#include "Objects.h"
#include <iostream>

Map::Map()
{
	blocks = list<Block>();
	bullets = list<Bullet>();
	toDraw = list<GameObject>();
};

void Map::Spawn() {

}

void Map::Tick() {

}
list<GameObject>& Map::ObjectsToDraw() {
	toDraw.clear();
	return toDraw;
}