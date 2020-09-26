#pragma once
#include "Enemy.h"

class HardEnemy : public Enemy
{
public:
	HardEnemy(float x, float y, Game* game);
	void update() override;
	int yMovFrecuencia;
	int frecuencia = 30;
};


