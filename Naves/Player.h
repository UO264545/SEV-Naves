#pragma once

#include "Actor.h"
#include "Projectile.h"
#include "Audio.h"

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	Player(float x, float y, std::string filename, int shootCadence, int rvx, int ry, Game* game);
	Projectile* shoot();
	void update();
	void moveX(float axis);
	void moveY(float axis);
	Audio* audioShoot;
	int rvx = 3;
	int rvy = 3;
	int shootCadence = 30;
	int shootTime = 0;
	int vidas = 3;
	int disparos = 10;
};

