#pragma once
#include "Objects.h"
#include "PolyMath.h"
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;


#pragma region Assets
enum AssetType {
	AssetMap,
	AssetVisual,
	AssetGameObject,
	AssetBlock
};

class Asset {
public:
	fs::path path;
};

class MapAsset : Asset {
public:
	fs::path background;
	int id;
	int terrain[16][8];
};
class VisualAsset : public Asset {
public:
	fs::path sprite;
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

class AssetManager {
private:
	std::string currentDir;
	std::string assetsDir = "\\assets\\";
	std::string blocksDir = assetsDir + "blocks\\";
	std::string mapsDir = assetsDir + "maps\\";

	std::string GetExtension(AssetType type);
public:
	VisualAsset border;
	VisualAsset farlands;
	VisualAsset logo;
	VisualAsset ending;

	GameObjectAsset positiveAbility;
	GameObjectAsset negativeAbility;

	GameObjectAsset bullet;
	GameObjectAsset player;

	BlockAsset blocks[3];
	MapAsset maps[5];

	AssetManager();
	void Setup();
	void GetBlockAssets(BlockAsset* result);
	void GetMapAssets(MapAsset* result);
	GameObjectAsset GetGameObjectAsset(std::string name);
	VisualAsset GetVisualAsset(std::string name);
	BlockAsset GetBlockAsset(fs::path path);
	MapAsset GetMapAsset(fs::path path);
	void ReadFile(std::string* result, fs::path filePath, int amount);
	void GetAssetPathsByType(fs::path* result, AssetType type, int amount);
	fs::path GetAssetPathByName(std::string name, AssetType type);
};