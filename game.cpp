

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

	Player player;

	//list<Map> maps;
	list<Bullet> bullets;
	list<Block> blocks;

	BoundingBox borderArea;
	BoundingBox mapArea;

	VisualObject border;
	VisualObject farlands;
	VisualObject logo;

	Vector2 blockSize;

	unsigned int score;
	int mapIndex;



#pragma region Drawing
	Sprite* GetSprite(fs::path Path) {
		return createSprite(Path.string().c_str());
	}
	void DrawVisualObject(VisualObject& object) {
		setSpriteSize(object.sprite, object.scale.x, object.scale.y);
		drawSprite(object.sprite, object.position.x, object.position.y);
	}
	void DrawGameObject(GameObject& object) {
		setSpriteSize(object.currentSprite, object.scale.x, object.scale.y);
		drawSprite(object.currentSprite, object.position.x, object.position.y);
	}

	void Draw() {
		drawTestBackground();

		for each (Block var in blocks)
		{
			DrawGameObject(var);
		}
		DrawOverlay();
	}

#pragma region Overlay

	void SetupOverlay() {
		border = VisualObject();
		border.sprite = GetSprite(assetManager.border.sprite);

		if (cmdScreen.x != cmdScreen.y) {
			logo = VisualObject();
			farlands = VisualObject();

			logo.sprite = GetSprite(assetManager.logo.sprite);
			farlands.sprite = GetSprite(assetManager.farlands.sprite);

			int farlandsScale = 1;
			int logoScale = 10;

			switch (cmdScreen.x > cmdScreen.y) {
			case true:
				farlandsScale = std::min(cmdScreen.x, cmdScreen.y);
				logoScale = cmdScreen.x - borderArea.MaxX();
				farlands.SetPosition(borderArea.b.x, borderArea.b.y);
				logo.SetPosition(borderArea.MaxX(), borderArea.MaxY() * 0.5 - (logoScale * 0.5));
				break;
			case false:
				farlandsScale = std::max(cmdScreen.x, cmdScreen.y);
				logoScale = cmdScreen.y - borderArea.MaxY();
				farlands.SetPosition(borderArea.c.x, borderArea.c.y);
				logo.SetPosition(borderArea.MaxX() * 0.5 - (logoScale * 0.5), borderArea.MaxY());
				break;
			}
			logo.SetScale(logoScale, logoScale);
			farlands.SetScale(farlandsScale, farlandsScale);
		}

		border.SetScale(borderArea.MaxX(), borderArea.MaxY());
	}
	void DrawOverlay() {
		if (farlands.sprite != nullptr) {
			DrawVisualObject(farlands);
			DrawVisualObject(logo);
		}
		DrawVisualObject(border);
	}
#pragma endregion
#pragma endregion

	void SetupVariables() {
		int squareWidth = std::min(cmdScreen.x, cmdScreen.y);
		borderArea = BoundingBox(Vector2(0, 0), Vector2(squareWidth, 0), Vector2(0, squareWidth), Vector2(squareWidth, squareWidth));
		mapArea = borderArea;
		mapArea.b.x *= 0.955;
		mapArea.d.x *= 0.955;
		mapArea.a.x = borderArea.b.x - mapArea.b.x;
		mapArea.a.y = mapArea.a.x;
		mapArea.b.y = mapArea.a.y;
		mapArea.c.x = mapArea.a.x;
		blockSize = Vector2(mapArea.MaxX()/12, mapArea.MaxY()/22);
	}

#pragma region Map
	void InstantiateMap(int& index) {
		bullets.clear();
		blocks.clear();
		MapAsset& map = assetManager.maps[index];
		int value = 0;
		for (int x = 0; x < 48; x++) {
			for (int y = 0; y < 12; y++) {
				value = map.terrain[x][y];
				if (value != 0) {
					InstantiateBlock(assetManager.blocks[value - 1], y, x);
				}
			}
		}
	}
	void DestroyMap() {

	}
	void NextMap() {
		DestroyMap();
		mapIndex++;
		InstantiateMap(mapIndex);
	}
#pragma endregion
	void InstantiateBlock(BlockAsset& asset, int x, int y) {
		Vector2 position = Vector2(mapArea.MinX() + x * blockSize.x, mapArea.MinY() + y * blockSize.y);
		Sprite* idleSprite = GetSprite(asset.idleSprite);
		Sprite* brokenSprite = GetSprite(asset.brokenSprite);
		Block block = Block(position, blockSize, idleSprite, brokenSprite, asset.id, asset.health);
		blocks.push_front(block);
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

		SetupVariables();
		SetupOverlay();

		int i = 0;
		InstantiateMap(i);
		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {
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
		mapIndex = -1;

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