#include "GameSystems.h"
#include <iostream>
#include <algorithm>

//void addBox(std::vector<Entity>& list, Vector2 startPos, Color color);
void removeBox(std::vector<Entity>& list);
void InitGameVariables(Box& ground, Box& grass, Box& playerTower, Box& enemyTower);

int main(void) {
	//GAME VARIABLES
	RenderSystem render; 
	MovementSystem updater; 
	EnemySpawner spawner; 
	MoneySystem money; 
	CombatSystem combat; 
	TestingFunctions testingFunctions; 
	TargetSystem targeting;
	ProjectileSystem projectiles;

	Box ground, grass, playerTower, enemyTower;
	int playerMoney = 0;
	int index = 0;
	State playerState = WALKING;
	Vector2 PlayerPos = { 0, 0 };
	Vector2 EnemyPos = { 0, 0 };
	float playerTime = 0.0f;
	int eHp = 0;
	//int enemyMoney = 0; for now

	// 2D Camera Variables
	Camera2D camera = { 0 };
	camera.zoom = 1.0f;
	camera.target = { 0.0f, -30.0f };
	int minOffset = 0;
	int maxOffset = WORLD_WIDTH - SCREEN_WIDTH;

	// Initializing Variables
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tower Defense Game");
	InitGameVariables(ground, grass, playerTower, enemyTower);
	SetTargetFPS(30);

	// Render Variables
	std::vector<Box> sceneObjects = { ground, grass, playerTower, enemyTower };
	std::vector<Entity> enemy;
	std::vector<Entity> player;
	std::vector<Entity> flying;
	std::vector<Projectile> projectileUnit;
	
	// Game Loop
	while (!WindowShouldClose()) {
		double dt = GetFrameTime();
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			Vector2 mouseDelta = GetMouseDelta();
			float dx = mouseDelta.x * (-1 / camera.zoom);
			camera.target.x += dx;
		}
		camera.target.x = std::clamp(camera.target.x, (float)minOffset, (float)maxOffset);

		// update MONEY
		money.updateMoney(dt);

		//------------------------------------------------------

		if (IsKeyPressed(KEY_F)) {
			testingFunctions.createPlayer(player);
		} 
		if (IsKeyPressed(KEY_J)) {
			testingFunctions.createEnemy(enemy);
		}
		if (IsKeyPressed(KEY_K)) {
			testingFunctions.createFlyEnemy(player);
		}
		
		// -----------------------------------------------------
		for (auto& p : player) {
			p.currentTargetIndex = targeting.getClosestEnemyIndex(p, enemy);
			//health.checkDead(unit);
			combat.processCombat(p, enemy, projectileUnit, dt);
		}

		for (auto& e : enemy) {
			e.currentTargetIndex = targeting.getClosestEnemyIndex(e, player);
			//health.checkDead(e);
			combat.processCombat(e, player, projectileUnit, dt);
		}

		//------------------------------------------------------
		
		removeBox(player);
		removeBox(enemy);
		projectiles.removeProjectiles(projectileUnit);
	
		//------------------------------------------------------

		updater.updatePosition(enemy, dt);
		updater.updatePosition(player, dt);
		if (!projectileUnit.empty()) {
			projectiles.processBombs(projectileUnit, enemy, dt);
		}

		//------------------------------------------------------
		
		BeginDrawing();
			ClearBackground(RAYWHITE);
			BeginMode2D(camera);

			render.drawBaseGame(sceneObjects);
			render.drawEntities(enemy);
			render.drawEntities(player);
			render.drawProjectiles(projectileUnit);
		
			EndMode2D();

			DrawText(TextFormat("current index %d", index), 100, 100, 1, BLACK);
		EndDrawing();
	}
	return 0;
}

void removeBox(std::vector<Entity>& list) {
	list.erase(
		std::remove_if(list.begin(), list.end(),
			[](const Entity& b) { return b.state == DEAD; }),
		list.end()
	);
}

void InitGameVariables(Box& ground, Box& grass, Box& playerTower, Box& enemyTower) {
	// x, y, width, height
	ground = { Rectangle{0.0f, GROUND_POSITION_Y, GROUND_WIDTH, GROUND_HEIGHT}, BROWN, 0 };
	grass = { Rectangle{0.0f, GROUND_POSITION_Y, GROUND_WIDTH, 20.0f}, GREEN, 0 };
	playerTower = { Rectangle{75, GROUND_POSITION_Y - 250.0f, 100, 250, }, YELLOW, 0 };
	enemyTower = { Rectangle{WORLD_WIDTH - 175, GROUND_POSITION_Y - 250.0f, 100, 250, }, RED, 0 };
}