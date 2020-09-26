#include "NormalEnemy.h"

NormalEnemy::NormalEnemy(float x, float y, Game* game)
	: Enemy("res/enemigo.png", x, y, 36, 40, new Animation("res/enemigo_movimiento.png", 36, 40, 108, 40, 6, 3, game), game) {
	
	vx = -1;
	vy = 0;
}
