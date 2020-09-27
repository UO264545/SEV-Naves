#pragma once

#include "Actor.h"
#include "Animation.h"

class Collectable : public Actor
{
public:
	Collectable(float x, float y, Game* game);
	void update();
	// El recolectable desaparece al cabo de cierto tiempo
	int timeToDisappear = 90;

	Animation* aMoving;
	Animation* animation;

	void draw() override;
};

