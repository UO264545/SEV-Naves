#include "Collectable.h"

Collectable::Collectable(float x, float y, Game* game) : 
	Actor("res/icono_recolectable.png", x, y, 40, 40, game) {
	vx = 0;
}

void Collectable::update() {
	timeToDisappear--;
}
