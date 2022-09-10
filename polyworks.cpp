

#include "Framework.h"
#include "PolyMath.h"
#include <iostream>
#include <regex>
#include "Objects.h"
#include "Managers.h"

using namespace std;

/* Test Framework realization */
class Polyworks : public Framework {
private:
	Vector2 targetScreenSize;
	Sprite* blueBlock;
	AssetManager assetManager;
public:
	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = targetScreenSize.x;
		height = targetScreenSize.y;
		fullscreen = false;
	}

	virtual bool Init() {
		assetManager = AssetManager();
		fs::path paths[3];
		assetManager.GetAssetPathsByType(paths, AssetBlock, sizeof(paths));
		std::string lines[24];
		assetManager.ReadFile(lines, paths[0], 3);
		blueBlock = createSprite("E:/dragonik/2022_win64/data/01-Breakout-Tiles.jpg");
		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {
		//Sprite* sprait = createSprite("E:/dragonik/2022_win64/data/01-Breakout-Tiles.jpg");
        drawTestBackground();
		setSpriteSize(blueBlock, 50, 50);
		drawSprite(blueBlock, 50, 50);

		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {

	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {

	}

	virtual void onKeyPressed(FRKey k) {
	}

	virtual void onKeyReleased(FRKey k) {
	}
	
	virtual const char* GetTitle() override
	{
		return "Arcanoid";
	}
	Polyworks(Vector2 screenSize) {
		targetScreenSize = screenSize;
	}
};
int main(int argc, char* argv[])
{
	Vector2 defaultScreenSize = Vector2(800, 600);
	Vector2 screenSize = defaultScreenSize;
	string command;
	switch (argc)
	{
	case 1:
		break;
	case 4:
		command = argv[1];
		if (command == "-window") {
			try {
				screenSize = Vector2(stoi(argv[2]), stoi(argv[3]));
				break;
			}
			catch (exception e) { cout << e.what(); }
		}
	default:
		cout << "Wrong command line syntax. Usage: \"polybroid.exe -window <width> <height>\"" << endl;
		break;

	}
	return run(new Polyworks(screenSize));
}