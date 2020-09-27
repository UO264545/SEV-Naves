#include "Collectable.h"

Collectable::Collectable(float x, float y, Game* game) :
	Actor("res/icono_recolectable.png", x, y, 40, 40, game) {
	vx = 0;

	aMoving = new Animation("res/recolectable.png", 32, 32, 256, 32, 3, 8, game);
	animation = aMoving;
}

void Collectable::update() {
	animation->update();
	timeToDisappear--;
}

void Collectable::draw() {
	animation->draw(x, y);
}