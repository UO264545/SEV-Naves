#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 50, 57, game) {
	audioShoot = new Audio("res/efecto_disparo.wav", false);
}

Player::Player(float x, float y, std::string filename, int shootCadence, int rvx, int rvy, Game* game)
	: Actor(filename, x, y, 50, 57, game) {
	audioShoot = new Audio("res/efecto_disparo.wav", false);
	this->shootCadence = shootCadence;
	this->rvx = rvx;
	this->rvy = rvy;
}

void Player::update() {
	if (shootTime > 0) {
		shootTime--;
	}
	x = x + vx;
	y = y + vy;
}

void Player::moveX(float axis) {
	vx = axis * rvx;
}

void Player::moveY(float axis) {
	vy = axis * rvy;
}

Projectile* Player::shoot() {
	if (shootTime == 0 && disparos > 0) {
		--disparos;
		audioShoot->play();
		shootTime = shootCadence;
		return new Projectile(x, y, game);
	}
	else {
		return NULL;
	}
}
