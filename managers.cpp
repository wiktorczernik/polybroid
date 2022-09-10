#include "Managers.h"
#include "Objects.h"
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;
using namespace std;

AssetManager::AssetManager() {
	currentDir = fs::current_path().string();
}
void AssetManager::GetAssetPathsByType(fs::path* result, AssetType type, int amount) {
	std::string ext = ".asset";
	std::string path = currentDir;
	switch (type)
	{
	case AssetGameObject:
		ext = ".gameObject";
		break;
	case AssetBlock:
		path += blocksDir;
		ext = ".block";
		break;
	case AssetMap:
		path += mapsDir;
		ext = ".map";
		break;
	default:
		break;
	}

	try {
		int count = 0;
		if (fs::exists(path) == false)
		{
			std::cout << "directory doesn't exist";
			return;
		}
		for (auto& p : fs::directory_iterator(path)) {
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

	}
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
		cout << "Can't read file: " << e.what();
		return result;
	}
}
void AssetManager::GetBlockAssets(BlockAsset* result) {
	fs::path paths[3];
	GetAssetPathsByType(paths, AssetBlock, 3);
	for each (fs::path path in paths)
	{
		BlockAsset block = GetBlockAsset(path);
		result[block.id] = block;
	}
}
void AssetManager::Setup() {
	GetBlockAssets(blocks);
}