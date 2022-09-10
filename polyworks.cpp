

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
	Sprite* blueBlock;
	AssetManager assetManager;
	BlockAsset blockAsset;
public:
	Vector2 targetScreenSize;
	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = targetScreenSize.x;
		height = targetScreenSize.y;
		fullscreen = false;
		assetManager = AssetManager();
	}

	virtual bool Init() {
		assetManager.Setup();
		blueBlock = createSprite(assetManager.blocks[2].idleSprite.string().c_str());
		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {
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
	Polyworks() {
		assetManager = AssetManager();
	}
};
int main(int argc, char* argv[])
{
	Polyworks* framework = new Polyworks();

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

	framework->targetScreenSize = screenSize;

	return run(framework);
}