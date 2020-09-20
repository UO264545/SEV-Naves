#pragma once

#include "Actor.h"

class Collectable : public Actor
{
public:
	Collectable(float x, float y, Game* game);
	void update();
	// El recolectable desaparece al cabo de cierto tiempo
	int timeToDisappear = 90;
};

