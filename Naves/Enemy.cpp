#include "Enemy.h"
Enemy::Enemy(std::string filename, float x, float y, float width, float height, Animation* aMoving, Game* game)
	: Actor(filename, x, y, width, height, game) { 

	this->aMoving = aMoving;
	animation = aMoving;
}
void Enemy::update() {
	// Actualizar la animación
	animation->update();

	x = x + vx;
	y = y + vy;
}

void Enemy::draw() {
	animation->draw(x, y);
}