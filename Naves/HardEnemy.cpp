#include "HardEnemy.h"

HardEnemy::HardEnemy(float x, float y, Game* game)
	: Enemy("res/enemigo.png", x, y, 36, 40, new Animation("res/enemigo_grande_mov.png", 36, 40, 108, 40, 6, 3, game), game) {

	yMovFrecuencia = frecuencia;
	vx = -2;
}

void HardEnemy::update(){
	yMovFrecuencia--;
	if (yMovFrecuencia == 0) {
		yMovFrecuencia = frecuencia;
		vy = 3;
	} else if (yMovFrecuencia == frecuencia/2) {
		vy = -3;
	}
	Enemy::update();
}