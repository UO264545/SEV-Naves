#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
		//llama al constructor del padre : Layer(renderer)
	init();
}

void GameLayer::init() {
	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	points = 0;
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.09, game);
	textPoints->content = std::to_string(points);

	delete player; //borra el jugador anterior
	player = new Player(50, 50, game);
	
	textVidas = new Text(std::to_string(player->vidas), WIDTH * 0.18, HEIGHT * 0.10, game);
	textDisparos = new Text(std::to_string(player->disparos), WIDTH * 0.18, HEIGHT * 0.9, game);

	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png", WIDTH * 0.85, HEIGHT * 0.1, 24, 24, game);
	backgroundVidas = new Actor("res/corazon.png", WIDTH * 0.10, HEIGHT * 0.10, 44, 36, game);
	backgroundDisparos = new Actor("res/icono_disparos.png", WIDTH * 0.1, HEIGHT * 0.9, 40, 40, game);
	
	projectiles.clear(); // Vaciar por si reiniciamos el juego

	enemies.clear(); // Vaciar por si reiniciamos el juego
	enemies.push_back(new HardEnemy(300, 50, game));
	enemies.push_back(new NormalEnemy(300, 200, game));
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
			textDisparos->content = std::to_string(player->disparos);
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

	background->update();
	// Generar enemigos
	newEnemyTime--;
	if (newEnemyTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		Enemy* enemy = nullptr;
		int enemyType = rand() % 100;
		if(enemyType < 60)
			enemy = new NormalEnemy(rX, rY, game);
		else
			enemy = new HardEnemy(rX, rY, game);
		enemies.push_back(enemy);
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

	if (collectable != nullptr)
		collectable->update();

	player->update();

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
			// Comprobar que no se ha chocado ya con el enemigo
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();
			if (!eInList) {
				deleteEnemies.push_back(enemy);
				if (--player->vidas == 0) {
					init();
					return; // Cortar el for
				}
				textVidas->content = std::to_string(player->vidas);
			}
		}
		checkColisionEnemyShoot(enemy, deleteEnemies, deleteProjectiles);
	}

	// Comprobamos colisión Player - Collectable
	// o el tiempo del Collectable ha finalizado
	if (collectable != nullptr) {
		if (collectable->timeToDisappear <= 0) {
			delete collectable;
			collectable = nullptr;
		}
		else if (player->isOverlap(collectable)) {
			player->disparos++;
			delete collectable;
			textDisparos->content = std::to_string(player->disparos);
			collectable = nullptr;
		}
	}

	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();

	// cout << "update GameLayer" << endl;
}

// Colisión Enemy - Shoot
void GameLayer::checkColisionEnemyShoot(Enemy* enemy, std::list<Enemy*> &deleteEnemies, std::list<Projectile*> &deleteProjectiles) {
	for (auto const& projectile : projectiles) {
		// Eliminar proyectiles que salen por la derecha
		if (!projectile->isInRender()) {
			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();
			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
		if (enemy->isOverlap(projectile)) {
			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();
			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();
			if (!eInList) {
				deleteEnemies.push_back(enemy);
			}
			points++;
			textPoints->content = std::to_string(points);
		}
	}
}

void GameLayer::draw() {
	//primero el background y después el player, sino no se ve el player
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

	textPoints->draw();
	backgroundPoints->draw(); //asi no lo tapa un enemigo
	backgroundVidas->draw();
	textVidas->draw();
	backgroundDisparos->draw();
	textDisparos->draw();

	SDL_RenderPresent(game->renderer); // Renderiza
}


