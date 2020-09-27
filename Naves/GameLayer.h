#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "NormalEnemy.h"
#include "HardEnemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Audio.h"
#include "Collectable.h"

#include <list>

#define PLAYER_1 1
#define PLAYER_2 2
#define NEW_COLLECTABLE_TIME 320 

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
	void changePlayer(int player);

	Player* player;
	Background* background;
	Actor* backgroundPoints;
	Actor* backgroundVidas;
	Actor* backgroundDisparos;
	Collectable* collectable;

	int newCollectableTime = NEW_COLLECTABLE_TIME;

	Audio* audioBackground;

	Text* textPoints;
	int points;

	Text* textVidas;

	Text* textDisparos;

	bool controlShoot = false;
	int controlMoveX = 0;
	int controlMoveY = 0;
	int newEnemyTime = 0;

	std::list<Enemy*> enemies;
	std::list<Projectile*> projectiles;

	int activePlayer = PLAYER_1;
};

