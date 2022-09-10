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
				std::cout << "Loaded block num." << count << " " << p.path() << '\n';
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
			cout << result[index] << '\n';
			index++;
		}
		file.close();
	}
}