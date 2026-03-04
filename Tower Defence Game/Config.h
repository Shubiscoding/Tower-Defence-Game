#pragma once

//-------SCREEN VARIABLES-------//
constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

//-------TOWER VARIABLES-------//
constexpr int TOWER_WIDTH = 50;
constexpr int TOWER_HEIGHT = 150;

//-------WORLD VARIABLES-------//
constexpr int WORLD_WIDTH = 3000;
constexpr int WORLD_HEIGHT = 720;

// -------GROUND VARIABLES-------
constexpr int GROUND_WIDTH = WORLD_WIDTH;
constexpr int GROUND_HEIGHT = 480;
constexpr float GROUND_POSITION_Y = 480.0f;

// -------ENITIY VARIABLES-------
constexpr float ENTITY_SPEED = 100.0f;

// -------SPAWN TIMER------------
constexpr float smallEnemyTimer = 2.0f;
constexpr float bigEnemyTimer = 6.0f;
constexpr float flyEnemyTimer = 4.0f;

// TYPE MATRIX
constexpr float typeMatrix[4][4] = {
			// GROUND, RANGED, FLYING, GIANT
/* GROUND*/	  {1.0f,	1.0f,   0.0f,   1.0f},
/*RANGED*/	  {1.0f,    0.9f,   1.5f,   1.0f},
/*FLYING*/	  {1.2f,    1.0f,   0.0f,   1.5f},
/*GIANT*/	  {1.1f,    1.1f,   0.0f,   1.2f}
};

// -------VARIABLES-------
constexpr Vector2 enemyGroundUnitPosition = { WORLD_WIDTH - 175, GROUND_POSITION_Y - 100.0f};
constexpr Vector2 playerGroundUnitPosition = { 75, GROUND_POSITION_Y - 100.0f };
constexpr Vector2 flyingUnitPosition = { WORLD_WIDTH - 200.0f, 200.0f};

// ------PRESETS-------

//PLAYER, GROUND, Rectangle{75, GROUND_POSITION_Y - 100.0f, 100.0f, 100.0f }, ORANGE, 100, 100, 20.0f, 2.0f, 0.0f, 110, -1, WALKING);
//ENEMY, GROUND,Rectangle{WORLD_WIDTH - 175, GROUND_POSITION_Y - 100.0f, 100.0f, 100.0f }, ORANGE, 100, 100.0f, 20.0f, 2.0f, 0.0f, 110, -1, WALKING); // Ground Enemies
//PLAYER, FLYING, Rectangle{WORLD_WIDTH - 175, GROUND_POSITION_Y, 100.0f, 100.0f }, BLUE, 70.0f, 70.0f, 0.0, 1.5f, 0.0f, 50, -1, WALKING); // Flying Enemies
//PLAYER, BOMB, Rectangle{WORLD_WIDTH - 175, GROUND_POSITION_Y, 50.0f, 50.0f }, BLACK, 50.0f, 30.0f, -1, WALKING); // Flying Enemies

// ---------PROJECTILE VARIABLE---------------
constexpr float DROP_SPEED = 50;