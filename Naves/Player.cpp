#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 50, 57, game) {
}

void Player::update() {
	if (shootTime > 0) {
		shootTime--;
	}
	
	int newX = x + vx;
	int newY = y + vy;
	if ((newX - this->width / 2) >= 0 && (newX + this->width / 2) <= WIDTH)
		x = newX;
	if ((newY - this->height / 2) >= 0 && (newY + this->height / 2) <= HEIGHT)
		y = newY;
}

void Player::moveX(float axis) {
	vx = axis * 3;
}

void Player::moveY(float axis) {
	vy = axis * 3;
}

Projectile* Player::shoot() {
	if (shootTime == 0 && availableShoots > 0) {
		shootTime = shootCadence;
		availableShoots--;
		return new Projectile(x, y, game);
	}
	else {
		return NULL;
	}
}
