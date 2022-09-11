

#include "Framework.h"
#include "PolyMath.h"
#include <iostream>
#include <regex>
#include "Objects.h"
#include "Assets.h"

using namespace std;

/* Test Framework realization */
class Game : public Framework {
private:
	AssetManager assetManager;

	list<Bullet> bullets;
	list<Block> blocks;

	list<GameObject> objects;
	BoundingBox borderArea;
	BoundingBox playableArea;

	Sprite* borderSprite;
	Sprite* farlandsSprite;
	Sprite* logoSprite;

	Vector2 farlandsPos;
	Vector2 logoPos;

	unsigned int score;
	unsigned int levelIndex;

	void InitMap() {

	}
	void NextMap() {

	}


	void Draw() {
		DrawOverlay();
	}
	void SetupOverlay() {
		int squareWidth = std::min(cmdScreen.x, cmdScreen.y);
		borderArea = BoundingBox(Vector2(0, 0), Vector2(squareWidth, 0), Vector2(0, squareWidth), Vector2(squareWidth, squareWidth));
		playableArea = borderArea;

		borderSprite = GetSprite(assetManager.border.sprite);
		if (cmdScreen.x != cmdScreen.y) {
			logoSprite = GetSprite(assetManager.logo.sprite);
			farlandsSprite = GetSprite(assetManager.farlands.sprite);

			int farlandsScale = 0;
			int logoScale = 0;

			switch (cmdScreen.x > cmdScreen.y) {
			case true:
				farlandsScale = std::min(cmdScreen.x, cmdScreen.y);
				logoScale = cmdScreen.x - borderArea.MaxX();
				farlandsPos = Vector2(borderArea.b.x, borderArea.b.y);
				logoPos = Vector2(borderArea.MaxX(), borderArea.MaxY()*0.5 - (logoScale * 0.5));
				break;
			case false:
				farlandsScale = std::max(cmdScreen.x, cmdScreen.y);
				logoScale = cmdScreen.y - borderArea.MaxY();
				farlandsPos = Vector2(borderArea.c.x, borderArea.c.y);
				logoPos = Vector2(borderArea.MaxX()*0.5 - (logoScale * 0.5), borderArea.MaxY());
				break;
			}
			setSpriteSize(logoSprite, logoScale, logoScale);
			setSpriteSize(farlandsSprite, farlandsScale, farlandsScale);
		}


		setSpriteSize(borderSprite, borderArea.MaxX(), borderArea.MaxY());
	}
	void DrawOverlay() {
		if (farlandsSprite) {
			drawSprite(farlandsSprite, farlandsPos.x, farlandsPos.y);
			drawSprite(logoSprite, logoPos.x, logoPos.y);
		}
		drawSprite(borderSprite, borderArea.MinX(), borderArea.MinY());
	}
public:
	Vector2 cmdScreen;

	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = cmdScreen.x;
		height = cmdScreen.y;
		fullscreen = false;
	}


	virtual bool Init() {
		assetManager.Setup();

		SetupOverlay();

		return true;
	}

	Sprite* GetSprite(fs::path Path) {
		return createSprite(Path.string().c_str());
	}

	virtual void Close() {

	}

	virtual bool Tick() {
        drawTestBackground();
		Draw();
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
		return "Polybroid";
	}
	Game() {
		score = 0;
		levelIndex = -1;

		assetManager = AssetManager();

		bullets = list<Bullet>();
		blocks = list<Block>();
	}
};
int main(int argc, char* argv[])
{
	Game* framework = new Game();

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
	if (screenSize.x < 400) screenSize.x = 400;
	if (screenSize.y < 400) screenSize.y = 400;
	framework->cmdScreen = screenSize;

	return run(framework);
}