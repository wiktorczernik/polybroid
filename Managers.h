#pragma once
#include "Objects.h"
#include "Framework.h"
#include "PolyMath.h"
#include <string>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

class AssetManager {
private:
	std::string currentDir;
	std::string assetsDir = "\\assets\\";
	std::string blocksDir = assetsDir + "blocks\\";
	std::string mapsDir = assetsDir + "maps\\";

public:
	BlockAsset blocks[3];
	AssetManager();
	void Setup();
	void GetBlockAssets(BlockAsset* result);
	BlockAsset GetBlockAsset(fs::path path);
	void ReadFile(std::string* result, fs::path filePath, int amount);
	void GetAssetPathsByType(fs::path* result, AssetType type, int amount);
};