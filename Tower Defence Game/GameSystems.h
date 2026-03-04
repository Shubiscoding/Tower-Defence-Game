#pragma once
#include <vector>
#include <time.h>
#include "Unit.h"
#include "Config.h"
#include <iostream>

using namespace std;

class RenderSystem {
public:
	void drawBaseGame(const vector<Box>& arr) {
		for (const auto& e : arr) {
			DrawRectangleRec(e.shape, e.color);
		}
	}

	void drawEntities(const vector<Entity>& arr) {
		for (const auto& e : arr) {
			DrawRectangleRec(e.shape, e.color);
		}
	}

	void drawProjectiles(const vector<Projectile>& arr) {
		if (arr.empty()) {
			return;
		}
		for (const auto& e : arr) {
			DrawRectangleRec(e.shape, e.color);
		}
	}
};

class MovementSystem {
public:
	void updatePosition(vector<Entity>& arr, float dt) {
		for (auto& e : arr) {
			if (e.state != WALKING) {
				continue;
			}
			int direction = (e.team == PLAYER) ? 1 : -1; 
			e.shape.x += ENTITY_SPEED * direction * dt;

			checkBounds(e);
		}
	}
	void checkBounds(Entity& unit) {
		if (unit.shape.x <= 0 || unit.shape.width + unit.shape.x >= WORLD_WIDTH) {
			unit.state = DEAD;
		}
	}
};

class TargetSystem {
public:
	int getClosestEnemyIndex(Entity& unit, vector<Entity>& enemies) {
		int bestScore = -1000000;
		int index = -1;
		for (int i = 0; i < enemies.size(); ++i) {
			if (enemies[i].state == DEAD) {
				continue;
			}
			float dx = unit.shape.x - enemies[i].shape.x;
			float dy = unit.shape.y - enemies[i].shape.y;

			if (unit.attackType == GROUND) {
				if (unit.team == PLAYER) {
					if (dx > 0) {
						continue;
					}
				}
				else {
					if (dx < 0) {
						continue;
					}
				}
			}
			float enemyDistance = dx * dx + dy * dy;
			float typeMultiplier = (unit.attackDamage * typeMatrix[unit.attackType][enemies[i].attackType] * 50);
			std::cout << typeMultiplier << endl;
			if (typeMultiplier == 0) {
				continue;
			}
			float enemyScore = typeMultiplier + (enemies[i].Maxhp - enemies[i].hp) * 2 - enemyDistance * 0.02f;

			if (bestScore < enemyScore) {
				bestScore = enemyScore;
				index = i;
			}
		}
		return index;
	}
};

class MoneySystem {
private:
	int money = 100;
	float elapsedTime = 0.0f;
	float spawnTimer = 4.0f;
public:
	void updateMoney(float dt) {
		elapsedTime += dt;
		if (elapsedTime >= spawnTimer) {
			elapsedTime = 0;
			money += 100;
		}
	}
	int getMoney() const {
		return money; 
	}
};

class EnemySpawner {
private:
	float smallElapsedTime = 0.0f;
	float bigElapsedTime = 0.0f;
	float flyElapsedTime = 0.0f;
	
public:
	
};

class CombatSystem {
private:
public:
	void updateCombat(vector<Entity>& unit, vector<Entity>& enemy, vector<Projectile>& projectiles, float dt) {
		for (auto& e : unit) {
			if (e.state == DEAD) {
				continue;
			}
			processCombat(e, enemy, projectiles, dt);
		}
	}

	void processCombat(Entity& unit, vector<Entity>& enemies, vector<Projectile>& projectiles, float dt) {
		if (!checkTargetValid(unit, enemies)) {
			unit.state = WALKING;
			return; 
		}
		if (unit.hp <= 0) {
			unit.state = DEAD;
			return;
		}
		if (checkInRange(unit, enemies[unit.currentTargetIndex])) {
			unit.state = ATTACK;
			switch (unit.attackType) {
			case GROUND:
				groundAttackTarget(unit, enemies, dt);
				break;
			case FLYING:
				flyingAttackTarget(unit, projectiles, dt);
				break;
			}
		} else {
			unit.state = WALKING;
		}
	}

	bool checkTargetValid(Entity& unit, vector<Entity>& enemies) {
		if (enemies.empty()) {
			return false;
		}
		else if (unit.currentTargetIndex < 0 || unit.currentTargetIndex >= enemies.size()){
			return false;
		}
		else if (enemies[unit.currentTargetIndex].state == DEAD) {
			return false;
		}
		return true;
	}

	bool checkInRange(Entity& unit, Entity& enemies) {
		float enemyCenterX = enemies.shape.x + (enemies.shape.width * 0.5);
		float playerCenterX = unit.shape.x + unit.shape.width * 0.5; 

		float dx = enemyCenterX - playerCenterX; 
			if (unit.team == PLAYER) {
				if (dx < 0) {
					return false;
				}
			}
			else {
				if (dx > 0) {
					return false;
				}
			}
		return (dx * dx) <= unit.attackRange * unit.attackRange;
	}
	
	void groundAttackTarget(Entity& unit, vector<Entity>& enemies, float dt) {
		unit.attackTimer += dt;
		if (unit.attackTimer >= unit.attackCooldown) {
			enemies[unit.currentTargetIndex].hp -= unit.attackDamage;
			unit.attackTimer = 0.0f;
		}
	}

	void flyingAttackTarget(Entity& unit, vector<Projectile>& bomb, float dt) {
		unit.attackTimer += dt;
		if (unit.attackTimer >= unit.attackCooldown) {
			bomb.push_back(Projectile{ PLAYER, BOMB, Rectangle{unit.shape.x, unit.shape.y, 50.0f, 50.0f }, BLACK, 50.0f, 30.0f, -1, WALKING});
			unit.attackTimer = 0.0f;	
		}
	}
};

class ProjectileSystem {
public:
	void removeProjectiles(vector<Projectile>& list) {
		list.erase(
			std::remove_if(list.begin(), list.end(),
				[](const Projectile& b) { return b.state == DEAD; }),
			list.end()
		);
	}

	void processBombs(vector<Projectile>& list, vector<Entity>& enemies,float dt) {
		for (auto& b : list) {
			switch (b.projectileType) {
			case BOMB:
				b.shape.y += DROP_SPEED * dt;
				if (b.shape.y >= GROUND_POSITION_Y - b.shape.width) {
					b.state = ATTACK;
					bombAttack(b, enemies);
					b.state = DEAD;
				}					
				break;
			case ARROW:
				float x = enemies[b.currentTargetIndex].shape.x - b.shape.x;
				float y = enemies[b.currentTargetIndex].shape.y - b.shape.y;
				float distance = sqrt(x * x + y * y);
				b.shape.x = x / distance;	
				b.shape.y = y / distance;

				if (b.shape.x >= enemies[b.currentTargetIndex].shape.x) {
					b.state = ATTACK;
					//arrowAttack();
					b.state = DEAD;
				}
				break;
			}
			
		}
	}

	void bombAttack(Projectile& p, vector<Entity>& enemies) {
		for (auto& e : enemies) {
			if (e.state == DEAD) {
				continue;
			}
			if (p.blastRadius + p.shape.x + p.shape.width < e.shape.x ||
				p.shape.x - p.blastRadius > e.shape.x) {
				continue;
			}
			e.hp -= p.attackDamage;
		}
	}

	void arrowAttack(Projectile& p, vector<Entity>& enemies) {
		if (enemies[p.currentTargetIndex].state == DEAD) {
			return;
		}
	}
};

class TestingFunctions {
public:
	void createPlayer(vector<Entity>& arr) {
		arr.push_back(Entity{ PLAYER, GROUND, Rectangle{ 75, GROUND_POSITION_Y - 100.0f, 100.0f, 100.0f }, ORANGE, 100, 100, 20.0f, 2.0f, 0.0f, 110, -1, WALKING });
	}

	void createEnemy(vector<Entity>& arr) {
		arr.push_back(Entity{ ENEMY, GROUND, Rectangle{ WORLD_WIDTH - 175, GROUND_POSITION_Y - 100.0f, 100.0f, 100.0f }, ORANGE, 100, 100.0f, 20.0f, 2.0f, 0.0f, 110, -1, WALKING });
	}

	void createFlyEnemy(vector<Entity>& arr) {
		arr.push_back(Entity{ PLAYER, FLYING, Rectangle{ 75, GROUND_POSITION_Y - 300.0f, 100.0f, 100.0f }, BLUE, 70.0f, 70.0f, 1.0, 1.5f, 0.0f, 100, -1, WALKING });
	}
};