#pragma once

#include "Actor.h"
#include "Animation.h"

class Enemy : public Actor
{
public:
	Enemy(std::string filename, float x, float y, float width, float height, Animation* aMoving, Game* game);
	virtual void update();
	void draw() override; // Sobreescribe

	Animation* aMoving;
	Animation* animation; // Referencia a la animación actual
};


