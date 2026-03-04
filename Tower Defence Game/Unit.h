#pragma once

#include <raylib.h>
#include <cstdint>

enum Team{
	PLAYER,
	ENEMY
};

enum ProjectileType {
	BOMB,
	ARROW
};

enum CombatType {
	GROUND = 0, 
	RANGE = 1,
	FLYING = 2, 
	GIANT = 3,
	HEALER = 4
};

enum State {
	WALKING,
	ATTACK,
	DEAD,
	DEFAULT
};

// ---Entities---
struct Entity {
	// Identity
	Team team; 
	CombatType attackType;

	// Transform
	Rectangle shape; 
	Color color;

	// Stats
	float hp;
	float Maxhp;
	float attackDamage;

	// Runtime Combat
	float attackCooldown;
	float attackTimer;
	float attackRange;
	int currentTargetIndex;
	State state;
};
// ---Entities---
struct Projectile {
	Team team; 
	ProjectileType projectileType;

	Rectangle shape;
	Color color;

	float blastRadius;
	float attackDamage;
	int currentTargetIndex;
	State state; // WALKING means falling
};
// ---Box---
struct Box {
	Rectangle shape;
	Color color;
	int id;  // 0: non-interactable, 1: interactable
};

