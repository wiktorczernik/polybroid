#include "Assets.h"
#include "Objects.h"
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
using namespace std;

BlockAsset::BlockAsset() {
	id = 1;
	health = 0;
}
AssetManager::AssetManager() {
	currentDir = fs::current_path().string();
}
std::string AssetManager::GetExtension(AssetType type) {
	std::string extension = ".";
	switch (type)
	{
	case AssetVisual:
		extension += "visual";
		return extension;
	case AssetMap:
		extension += "map";
		return extension;
	case AssetGameObject:
		extension += "gameObject";
		return extension;
	case AssetBlock:
		extension += "block";
		return extension;
	default:
		extension += "asset";
		return extension;
	}
}


void AssetManager::GetAssetPathsByType(fs::path* result, AssetType type, int amount) {
	std::string ext = GetExtension(type);
	std::string path = currentDir + assetsDir;
	try {
		int count = 0;
		if (fs::exists(path) == false)
		{
			std::cout << "directory doesn't exist";
			return;
		}
		for (auto& p : fs::recursive_directory_iterator(path)) {
			if (count >= amount) {
				return;
			}
			if (p.path().extension() == ext) {
				result[count] = p.path();
				count++;
				std::cout << "Loaded asset " << p.path() << '\n';
			}
		}
	}
	catch (__std_win_error e) {
		std::cout << "CRITICAL ASSET MANAGER ERROR" << "\n\n\n";
	}
}
fs::path AssetManager::GetAssetPathByName(std::string name, AssetType type) {
	std::string ext = GetExtension(type);
	std::string path = currentDir + assetsDir;
	try {
		if (!fs::exists(path))
		{
			std::cout << "directory doesn't exist";
			return path;
		}
		for (auto& p : fs::recursive_directory_iterator(path)) {
			if (p.path().filename() == name + ext) {
				std::cout << "Loaded asset " << p.path() << '\n';
				return p.path();
			}
		}
	}
	catch (__std_win_error e) {
		std::cout << "CRITICAL ASSET MANAGER ERROR" << "\n\n\n";
	}
	return path;
}
void AssetManager::ReadFile(std::string* result, fs::path filePath, int amount) {
	std::fstream file;
	file.open(filePath, ios::in);
	int index = 0;
	if (file.is_open()) {
		while (getline(file, result[index])) {
			if (index>=amount){
				break;
			}
			index++;
		}
		file.close();
	}
}

MapAsset AssetManager::GetMapAsset(fs::path path) {
	std::string lines[18];
	MapAsset result = MapAsset();
	fs::path background = path.parent_path();
	try {
		ReadFile(lines, path, 18);

		background += "\\" + lines[1];

		result.id = std::stoi(lines[0]);
		result.background = background;
		int value = 0;
		for (int x = 2; x < 18; x++) {
			for (int y = 0; y < 8; y++) {
				value = std::stoi(string() + lines[x].at(y));
				result.terrain[x - 2][y] = value;
			}
		}
		return result;
	}
	catch (std::exception& e) {
		cout << "Can't read file: " << e.what();
		return result;
	}
}
BlockAsset AssetManager::GetBlockAsset(fs::path path) {
	std::string lines[4];
	int id;
	int health;
	fs::path idleSprite = path.parent_path();
	fs::path brokenSprite = path.parent_path();
	BlockAsset result = BlockAsset();
	try {
		ReadFile(lines, path, 4);

		id = std::stoi(lines[0]);
		health = std::stoi(lines[1]);

		idleSprite += "\\" + lines[2];
		brokenSprite += "\\" + lines[3];

		result.id = id;
		result.path = path;
		result.health = health;
		result.idleSprite = idleSprite;
		result.brokenSprite = brokenSprite;

		return result;
	}
	catch(std::exception& e) {
		result.id = 1;
		cout << "Can't read file: " << e.what();
		return result;
	}
}
VisualAsset AssetManager::GetVisualAsset(std::string name) {
	std::string lines[49];
	fs::path path = GetAssetPathByName(name, AssetVisual);
	fs::path sprite = path.parent_path();
	VisualAsset result = VisualAsset();
	try {
		ReadFile(lines, path, 49);

		sprite += "\\" + lines[0];
		result.path = path;
		result.sprite = sprite;

		return result;
	}
	catch (std::exception& e) {
		cout << "Can't read file: " << e.what();
		return result;
	}
}
GameObjectAsset AssetManager::GetGameObjectAsset(std::string name) {
	std::string lines[1];
	fs::path path = GetAssetPathByName(name, AssetGameObject);
	fs::path sprite = path.parent_path();
	GameObjectAsset result = GameObjectAsset();
	try {
		ReadFile(lines, path, 1);

		sprite += "\\" + lines[0];
		result.path = path;
		result.idleSprite = sprite;

		return result;
	}
	catch (std::exception& e) {
		cout << "Can't read file: " << e.what();
		return result;
	}
}
void AssetManager::GetBlockAssets(BlockAsset* result) {
	fs::path paths[3];
	GetAssetPathsByType(paths, AssetBlock, 3);
	int index = 0;
	for each (fs::path path in paths)
	{
		BlockAsset block = GetBlockAsset(path);
		index = block.id - 1;
		if (index >= sizeof(result)) {
			std::cout << "CONTINUE" << '\n';
			std::cout << "CONTINUE" << '\n';
			continue;
		}
		result[index] = block;
	}
}
void AssetManager::GetMapAssets(MapAsset* result) {
	fs::path paths[3];
	GetAssetPathsByType(paths, AssetMap, 3);
	int index = 0;
	for each (fs::path path in paths)
	{
		MapAsset block = GetMapAsset(path);
		index = block.id - 1;
		if (index >= sizeof(result)) {
			std::cout << "CONTINUE" << '\n';
			continue;
		}
		result[index] = block;
	}
}
void AssetManager::Setup() {
	GetBlockAssets(blocks);
	GetMapAssets(maps);

	positiveAbility = GetGameObjectAsset("positiveAB");
	negativeAbility = GetGameObjectAsset("negativeAB");
	bullet = GetGameObjectAsset("bullet");
	player = GetGameObjectAsset("player");

	border = GetVisualAsset("border");
	farlands = GetVisualAsset("farlands");
	logo = GetVisualAsset("logo");
}