

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
	Bullet bullet;
	list<GameObject> objects;
	BoundingBox canvas;
public:
	Vector2 cmdScreen;

	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = cmdScreen.x;
		height = cmdScreen.y;
		fullscreen = false;
	}

	virtual bool Init() {
		objects = list<GameObject>();
		assetManager.Setup();
		int size = std::min(cmdScreen.x, cmdScreen.y);
		canvas = BoundingBox(Vector2(0, 0), Vector2(size, 0), Vector2(0, size), Vector2(size, size));
		Sprite* sprite = createSprite(assetManager.blocks[0].idleSprite.string().c_str());
		Sprite* spriteTwo = createSprite(assetManager.blocks[1].idleSprite.string().c_str());
		GameObject object = GameObject(Vector2(330, 300), Vector2(120, 50), spriteTwo);
		GameObject objectTwo = GameObject(Vector2(500, 250), Vector2(120, 40), spriteTwo);
		objects.push_back(object);
		objects.push_front(objectTwo);
		bullet = Bullet(canvas, Vector2(250, 350), Vector2(50, 50), Vector2(3, -3), sprite);

		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {
        drawTestBackground();
		//std::cout << clock() << '\n';
		//bullet.Tick();

		bool invert[2];

		bullet.Tick(objects);

		setSpriteSize(bullet.currentSprite, bullet.scale.x, bullet.scale.y);
		drawSprite(bullet.currentSprite, bullet.position.x, bullet.position.y);
		setSpriteSize(objects.back().currentSprite, objects.back().scale.x, objects.back().scale.y);
		drawSprite(objects.back().currentSprite, objects.back().position.x, objects.back().position.y);

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

	framework->cmdScreen = screenSize;

	return run(framework);
}