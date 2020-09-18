#include "Enemy.h"
Enemy::Enemy(float x, float y, Game* game)
	: Actor("res/enemigo.png", x, y, 36, 40, game) {
	vx = 1;
}
void Enemy::update() {
	x = x + vx;
	if (x - width / 2 <= 0 || x + width / 2 >= WIDTH) {
		vx = vx * -1;
	}
}