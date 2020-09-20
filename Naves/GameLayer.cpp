#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
		//llama al constructor del padre : Layer(renderer)
	init();
}

void GameLayer::init() {
	delete player; //borra el jugador anterior
	player = new Player(50, 50, game);
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, game);

	projectiles.clear(); // Vaciar por si reiniciamos el juego

	enemies.clear(); // Vaciar por si reiniciamos el juego
	enemies.push_back(new Enemy(300, 50, game));
	enemies.push_back(new Enemy(300, 200, game));
}

void GameLayer::processControls() {
	//obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}

	// Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			projectiles.push_back(newProjectile);
		}
	}
	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}
	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
	}
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_QUIT) {
		game->loopActive = false;
	}

	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_1:
			game->scale();
			break;
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		}
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}
	}
}

void GameLayer::update() {
	using namespace std;

	// Generar enemigos
	newEnemyTime--;
	if (newEnemyTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new Enemy(rX, rY, game));
		newEnemyTime = 110;
	}

	// Generar recolectable
	newCollectableTime--;
	if (newCollectableTime <= 0) {
		int rX = (rand() % (WIDTH - 20)) + 1 + 10;
		int rY = (rand() % (HEIGHT - 20)) + 1 + 10;
		collectable = new Collectable(rX, rY, game);
		newCollectableTime = NEW_COLLECTABLE_TIME;
	}

	player->update();
	if(collectable != nullptr)
		collectable->update();
	for (auto const& enemy : enemies) {
		enemy->update();
	}
	for (auto const& projectile : projectiles) {
		projectile->update();
	}

	// Colisiones
	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;
	
	for (auto const& enemy : enemies) {
		// Eliminar enemigos que salen por la izquierda
		if (enemy->x < 0)
			deleteEnemies.push_back(enemy);
		if (player->isOverlap(enemy)) {
			init();
			return; // Cortar el for
		}
		checkColisionEnemyShoot(enemy, deleteEnemies, deleteProjectiles);
	}

	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
	}
	deleteProjectiles.clear();

	// Comprobamos colisi�n Player - Collectable
	// o el tiempo del Collectable ha finalizado
	if (collectable != nullptr) {
		if (collectable->timeToDisappear <= 0) {
			delete collectable;
			collectable = nullptr;
		} else if (player->isOverlap(collectable)) {
			player->availableShoots++;
			delete collectable;
			collectable = nullptr;
		}
	}

	//cout << "update GameLayer" << endl;
}

// Colisi�n Enemy - Shoot
void GameLayer::checkColisionEnemyShoot(Enemy* enemy, std::list<Enemy*> &deleteEnemies, std::list<Projectile*> &deleteProjectiles) {
	for (auto const& projectile : projectiles) {
		// Eliminar projectiles que salen por la derecha
		if (projectile->x > WIDTH)
			deleteProjectiles.push_back(projectile);
		if (enemy->isOverlap(projectile)) {
			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();
			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
			enemy->life--;
			if (enemy->life == 0) {
				bool eInList = std::find(deleteEnemies.begin(),
					deleteEnemies.end(),
					enemy) != deleteEnemies.end();
				if (!eInList) {
					deleteEnemies.push_back(enemy);
				}
			}
		}
	}
}

void GameLayer::draw() {
	//primero el background y despu�s el player, sino no se ve el player
	background->draw();
	player->draw();
	
	if (collectable != nullptr)
		collectable->draw();

	for (auto const& enemy : enemies) {
		enemy->draw();
	}

	for (auto const& projectile : projectiles) {
		projectile->draw();
	}
	drawShoots();
	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::drawShoots() {
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("res/sans.ttf", 16);

	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255; //transparente

	std::string content = "Disparos: " + std::to_string(player->availableShoots);
	SDL_Surface* surface = TTF_RenderText_Blended(font, content.c_str(), color);
	// c_str() transforma el string a cost *char;
	SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, surface);

	SDL_Rect rect;
	rect.x = 10;
	rect.y = 10 - surface->h / 2;
	rect.w = surface->w;
	rect.h = surface->h;

	SDL_FreeSurface(surface);
	SDL_RenderCopy(game->renderer, texture, NULL, &rect);
	SDL_DestroyTexture(texture);

}
