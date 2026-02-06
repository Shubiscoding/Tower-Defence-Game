#include "Unit.h"
#include "Config.h"
#include <vector>
#include <raymath.h>
#include <rlgl.h>

void addBox(std::vector<Box>& list, Vector2 startPos, Color color);
void removeBox(std::vector<Box>& list);

int main(void) {
	//GAME VARIABLES
	Vector2 g_size = { WORLD_WIDTH, 120.0f };
	Vector2 groundPos = { 0.0f, WORLD_HEIGHT - g_size.y - 20.0f};
	Vector2 grass_size = { WORLD_WIDTH, 40.0f };
	Vector2 grassPos = { 0.0f, WORLD_HEIGHT - g_size.y - 40.f };
	std::vector<Box> Entities; 

	// UI
	Vector2 ui_size = { 100.0f, 100.0f };
	Vector2 ui_pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT - ui_size.y - 20.0f };
	
	// TOWER
	Vector2 t_size = { 100, 250 };
	Vector2 playerTos = { 75, SCREEN_HEIGHT - t_size.y - 120.0f };
	Vector2 enemyTPos = {WORLD_WIDTH - t_size.x - 75, SCREEN_HEIGHT - t_size.y - 120.0f};

	// 2D Camera
	Camera2D camera = { 0 };
	camera.zoom = 1.0f;
	camera.target = { 0.0f, -30.0f };
	int minOffset = 0;
	int maxOffset = WORLD_WIDTH - SCREEN_WIDTH;

	// Attack Box
	Vector2 boxSize = { 100.0f, 100.0f };

	// UI
	Box p_attack(ui_size, Vector2{ ui_pos.x - 150.0f, ui_pos.y }, WHITE);
	Box p_fly(ui_size, ui_pos, WHITE);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tower Defense Game");
	Box Ground(g_size, groundPos, DARKBROWN);
	Box Grass(grass_size, grassPos, GREEN);
	Box PlayerTower(t_size, playerTos, BLUE);
	Box EnemyTower(t_size, enemyTPos, RED);
	
	SetTargetFPS(30);

	while (!WindowShouldClose()) {
		Vector2 MousePos = GetMousePosition();

		// This moves the camera 
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {			
			Vector2 mouseDelta = GetMouseDelta();
			int dx = mouseDelta.x * ( - 1 / camera.zoom);
			camera.target.x += dx; 
		}
		camera.target.x = Clamp(camera.target.x, minOffset, maxOffset);

		// UI 
		if (p_attack.checkCollisions(MousePos)) {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
				addBox(Entities, Vector2{ playerTos.x + 100.0f, playerTos.y }, DARKGREEN);
		}
		else if (p_fly.checkCollisions(MousePos)) {
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			addBox(Entities, Vector2{ playerTos.x + 100.0f, playerTos.y - 400.0f }, YELLOW);
		}

		removeBox(Entities);
		// Drawing
		BeginDrawing();
			ClearBackground(RAYWHITE);
			// 2d Drawing
			BeginMode2D(camera);
				PlayerTower.draw();
				EnemyTower.draw();
				Grass.draw();
				Ground.draw();

				for (int i = 0; i < Entities.size(); ++i) {
					Entities[i].draw();
				}
			EndMode2D();
			// 1d Drawing
			p_attack.draw();
			p_fly.draw();
			// Text
			//DrawText("The window is loaded!", 100, 50, 20, LIGHTGRAY);
			DrawText(TextFormat("Camera current position %f%f", camera.target.x, camera.target.y), 200, 60, 20, BLUE);
		EndDrawing();
	}
	return 0;
}

void addBox(std::vector<Box>& list, Vector2 startPos, Color color){
	list.emplace_back(Vector2{ 100.0f, 100.0f }, startPos, color);
}

void removeBox(std::vector<Box>& list) {
	list.erase(
		std::remove_if(list.begin(), list.end(),
			[](const Box& b) { return !b.alive; }),
		list.end()
	);
}
