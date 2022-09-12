

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
	list<Bullet> bullets;
	list<Block> blocks;
	list<Ability> abilities;

	BoundingBox borderArea;
	BoundingBox mapArea;

	VisualObject border;
	VisualObject farlands;
	VisualObject logo;
	VisualObject background;

	Vector2 blockSize;
	Vector2 abilitySize;
	Vector2 bulletSize;
	Vector2 playerSize;

	float abilitySpawnDelay;
	float abilitySpawnCooldown;

	unsigned int score;
	int mapIndex;
	int blocksToDestroy;

	int moveInput;

	Timer abilityTimer;
	bool canAbilitySpawn;

	int deltaTime;
	clock_t previousTickTime;
	clock_t currentTickTime;

	void GameTick() {
		player.UpdateInput(moveInput, false, Vector2(0, 0));

		player.Tick();

		abilityTimer.Tick(deltaTime);

		if (canAbilitySpawn) {
			InstantiateAbility();
			cout << "\n\n" << "ABILITY CAN SPAWN" << "\n\n";
		}
		auto begin = bullets.cbegin();


		PhysTick();
		DestroyDeadmen();

		if (blocksToDestroy <= 0) {
			NextMap();
		}
	}
	void PhysTick() {
		for (auto& a : abilities) {
			a.Tick(player);
		}
		for (auto& b : bullets)
		{
			b.Tick(blocks, player);
		}
	}

	void DestroyDeadmen() {

		//HERE COMES THE HARDCODE BABY

		std::list<Block>::iterator bi = blocks.begin();

		list<Block>::const_iterator bitr = blocks.cbegin();
		while (bitr != blocks.cend())
		{
			list<Block>::const_iterator curr = bitr++;
			if (curr->IsAlive == false) {
				destroySprite(curr->idleSprite);
				score += 150 * (mapArea.MaxY() / curr->position.y);
				blocksToDestroy--;

				blocks.erase(curr);
			}
		}

		std::list<Bullet>::iterator i = bullets.begin();

		list<Bullet>::const_iterator itr = bullets.cbegin();
		while (itr != bullets.cend())
		{
			list<Bullet>::const_iterator curr = itr++;
			if (curr->IsAlive == false) {
				destroySprite(curr->idleSprite);

				bullets.erase(curr);
			}
		}

		list<Ability>::const_iterator aitr = abilities.cbegin();
		while (aitr != abilities.cend())
		{
			list<Ability>::const_iterator curr = aitr++;
			if (curr->IsAlive == false) {
				destroySprite(curr->idleSprite);
				abilities.erase(curr);
			}
		}
	}

#pragma region Drawing
	Sprite* GetSprite(fs::path Path) {
		return createSprite(Path.string().c_str());
	}
	void DrawVisualObject(VisualObject& object) {
		setSpriteSize(object.sprite, object.scale.x, object.scale.y);
		drawSprite(object.sprite, object.position.x, object.position.y);
	}
	void DrawGameObject(GameObject& object) {
		if (object.currentSprite == nullptr) {
			return;
		}
		setSpriteSize(object.currentSprite, object.scale.x, object.scale.y);
		drawSprite(object.currentSprite, object.position.x, object.position.y);
	}

	void Draw() {
		drawTestBackground();
		DrawVisualObject(background);

		for each (Block block in blocks)
		{
			DrawGameObject(block);
		}
		for each (Ability ability in abilities) {
			DrawGameObject(ability);
		}
		for each (Bullet bullet in bullets)
		{
			DrawGameObject(bullet);
		}
		DrawGameObject(player);
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
		score = 0;
		blocksToDestroy = 0;
		mapIndex = -1;

		moveInput = 0;

		abilityTimer.Setup(&canAbilitySpawn, 3000, true);

		int squareWidth = std::min(cmdScreen.x, cmdScreen.y);
		borderArea = BoundingBox(Vector2(0, 0), Vector2(squareWidth, 0), Vector2(0, squareWidth), Vector2(squareWidth, squareWidth));
		mapArea = borderArea;

		mapArea.b.x *= 0.955;
		mapArea.d.x *= 0.955;
		mapArea.a.x = borderArea.b.x - mapArea.b.x;
		mapArea.c.x = mapArea.a.x;
		
		mapArea.a.y = mapArea.a.x;
		mapArea.b.y = mapArea.a.y;
		
		background = VisualObject();
		background.SetPosition(mapArea.MinX(), mapArea.MinY());
		background.SetScale(mapArea.MaxX(), mapArea.MaxY());

		blockSize = Vector2((mapArea.MaxX()-mapArea.MinX()) / 8, (mapArea.MaxY() - mapArea.MinX()) / 16);
		abilitySize = Vector2(blockSize.x, blockSize.x);
		bulletSize = Vector2(blockSize.y, blockSize.y);
		playerSize = Vector2(blockSize.x*2, blockSize.y);
	}

#pragma region Map
	void InstantiateMap(int& index) {
		MapAsset& map = assetManager.maps[index];

		background.sprite = GetSprite(map.background);

		int value = 0;
		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 8; y++) {
				value = map.terrain[x][y];
				if (value != 0) {
					BlockAsset& asset = assetManager.blocks[value - 1];
					if (asset.health < 100) {
						blocksToDestroy++;
					}
					InstantiateBlock(asset, y, x);
				}
			}
		}

		abilityTimer.Restart();
		InstantiatePlayer();
		InstantiateBullet(Vector2(200, 500));

	}
	void DestroyMap() {
		bullets.clear();
		blocks.clear();
	}
	void NextMap() {
		DestroyMap();
		mapIndex++;
		InstantiateMap(mapIndex);
	}
#pragma endregion
	void InstantiatePlayer() {
		GameObjectAsset& asset = assetManager.player;
		Vector2 position = Vector2(mapArea.MaxX() / 2 - (playerSize.x/2), mapArea.MaxY() * 0.95 - (playerSize.y));
		Sprite* sprite = GetSprite(asset.idleSprite);
		Player pl = Player(mapArea, position, playerSize, Vector2(3, 0), sprite);
		player = pl;
	}
	void InstantiateBlock(BlockAsset& asset, int x, int y) {
		Vector2 position = Vector2(mapArea.MinX() + x * blockSize.x, mapArea.MinY() + y * blockSize.y);
		Sprite* idleSprite = GetSprite(asset.idleSprite);
		Sprite* brokenSprite = GetSprite(asset.brokenSprite);
		Block block = Block(position, blockSize, idleSprite, brokenSprite, asset.id, asset.health);
		blocks.push_front(block);
	}
	void InstantiateBullet(Vector2 Position) {
		GameObjectAsset& asset = assetManager.bullet;
		Sprite* idleSprite = GetSprite(asset.idleSprite);
		Bullet bullet = Bullet(mapArea, Position, bulletSize, Vector2(2, -2), idleSprite);
		bullets.push_front(bullet);
	}
	void InstantiateAbility() {
		GameObjectAsset& asset = assetManager.blocks[0];
		int pseudoRand = rand() % 3 - 3;
		bool IsPositive = (rand() % 10) >= 6 ? true : false;
		Vector2 position = Vector2(mapArea.MaxX()/2 - abilitySize.x/2, mapArea.MinY()-abilitySize.y);

		Sprite* Sprite;
		switch (IsPositive)
		{
		case true:
			Sprite = GetSprite(assetManager.positiveAbility.idleSprite);
			break;
		case false:
			Sprite = GetSprite(assetManager.negativeAbility.idleSprite);
			break;
		}
		Ability ability = Ability(mapArea, position, abilitySize, Vector2(2/pseudoRand, 2), IsPositive, Sprite);
		
		abilities.push_front(ability);
		//currentAbility = ability;
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
		NextMap();
		previousTickTime = clock();
		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {
		currentTickTime = clock();
		deltaTime = (int)(currentTickTime - previousTickTime);
		GameTick();
		Draw();
		
		previousTickTime = currentTickTime;
		return false;
	}
	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {

	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {

	}

	virtual void onKeyPressed(FRKey k) {
		switch (k)
		{
		case FRKey::RIGHT:
			moveInput = 1;
			break;
		case FRKey::LEFT:
			moveInput = -1;
			break;
		case FRKey::UP:
			NextMap();
			break;
		default:
			break;
		}
	}

	virtual void onKeyReleased(FRKey k) {
		switch (k)
		{
		case FRKey::RIGHT:
			moveInput = moveInput > 0 ? 0 : moveInput;
			break;
		case FRKey::LEFT:
			moveInput = moveInput < 0 ? 0 : moveInput;
			break;
		default:
			break;
		}
	}
	
	virtual const char* GetTitle() override
	{
		return "Polybroid";
	}
	Game() {
		assetManager = AssetManager();

		bullets = list<Bullet>();
		blocks = list<Block>();

		abilityTimer = Timer();
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