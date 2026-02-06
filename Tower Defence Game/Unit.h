#pragma once
#include <raylib.h>
#include <cstdint>

class Box {
private:
	Vector2 m_size;
	Vector2 m_position;
	Rectangle rect = { m_size.x, m_size.y, m_position.x, m_position.y };
	Color m_color;
	int m_hp; 

public:
	bool alive = false;
	Box(Vector2 size, Vector2 position, Color color) :
		m_size(size), m_position(position), m_color(color), alive(true){};

	void draw() const {
		DrawRectangle(m_position.x, m_position.y, m_size.x, m_size.y, m_color); 
	}

	void updatePos(int x, int y, float dt){
		m_position.x += x * dt;
		m_position.y += y * dt; 
	}
	bool checkCollisions(Vector2 object_pos) const {
		return CheckCollisionPointRec(object_pos, rect);
	}
};