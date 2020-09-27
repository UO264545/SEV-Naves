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
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = std::to_string(points);

	delete player; //borra el jugador anterior
	player = new Player(50, 50, game);
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png", WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);

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
		// Cambio de nave
		case SDLK_2:
			changePlayer(PLAYER_1);
			break;
		case SDLK_3:
			changePlayer(PLAYER_2);
			break;
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

	for (auto const& enemy : enemies) {
		enemy->draw();
	}

	for (auto const& projectile : projectiles) {
		projectile->draw();
	}

	textPoints->draw();
	backgroundPoints->draw(); //asi no lo tapa un enemigo

	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::changePlayer(int playerNum) {
	if (playerNum == activePlayer)
		return;
	int x = player->x;
	int y = player->y;
	delete player;
	switch (playerNum) {
	case PLAYER_1:
		player = new Player(x, y, game);
		activePlayer = PLAYER_1;
		break;
	case PLAYER_2:
		player = new Player(x, y, "res/jugador2.png", 20, 4, 4, game);
		activePlayer = PLAYER_2;
		break;
	default:
		break;
	}
}


