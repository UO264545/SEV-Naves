#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Collectable.h"

#include <list>

#define NEW_COLLECTABLE_TIME 120 

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void checkColisionEnemyShoot(Enemy* enemy, std::list<Enemy*> &deleteEnemies, std::list<Projectile*> &deleteProjectiles);
	void draw() override;
	void keysToControls(SDL_Event event);
	void drawShoots();

	Player* player;
	Background* background;
	bool controlShoot = false;
	int controlMoveX = 0;
	int controlMoveY = 0;
	int newEnemyTime = 0;
	int newCollectableTime = NEW_COLLECTABLE_TIME;

	std::list<Enemy*> enemies;
	std::list<Projectile*> projectiles;
	Collectable* collectable; // Un único recolectable
};

