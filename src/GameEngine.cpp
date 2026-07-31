#include "GameEngine.h"

#define VIRTUAL_RES_WIDTH 160
#define VIRTUAL_RES_HEIGHT 100

GameEngine::GameEngine() {
}

GameEngine::~GameEngine() {
}

void GameEngine::init(const char* title, int width, int height) {
	if (SDL_Init(SDL_INIT_AUDIO || SDL_INIT_VIDEO || SDL_INIT_EVENTS) == 1) {
		std::cout << "Subsystems initialized!..." << std::endl;

		window = SDL_CreateWindow(title, width, height, 0);
		if (window) {
			std::cout << "Window created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, 0);
		if (renderer) {
			SDL_SetRenderLogicalPresentation(renderer, VIRTUAL_RES_WIDTH, VIRTUAL_RES_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
			std::cout << "Renderer created!" << std::endl;
		}

		isRunning = true;
	} else {
		isRunning = false;
	}

	Object a1(20.0f, 20.0f, 8.0f, -6.0f, 16, 0.0f);
	vecAsteroids.push_back( a1 );

	player.x = VIRTUAL_RES_WIDTH / 2.0f;
	player.y = VIRTUAL_RES_HEIGHT / 2.0f;
	player.dx = 0.0f;
	player.dy = 0.0f;
	player.angle = 0.0f;
}

void GameEngine::handleEvents() {
	while(SDL_PollEvent(&event)){
		if (event.type == SDL_EVENT_QUIT) {
			isRunning = false;
			break;
		}
		if (event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.scancode == SDL_SCANCODE_ESCAPE){
				isRunning = false;
				break;
			}
			if (event.key.scancode == SDL_SCANCODE_UP || event.key.scancode == SDL_SCANCODE_W) {
				key_state[UP] = true;
			}
			if (event.key.scancode == SDL_SCANCODE_LEFT || event.key.scancode == SDL_SCANCODE_A) {
				key_state[LEFT] = true;
			}
			else if (event.key.scancode == SDL_SCANCODE_RIGHT || event.key.scancode == SDL_SCANCODE_D) {
				key_state[RIGHT] = true;
			}
		}
		if (event.type == SDL_EVENT_KEY_UP) {
			if (event.key.scancode == SDL_SCANCODE_UP || event.key.scancode == SDL_SCANCODE_W) {
				key_state[UP] = false;
			}
			if (event.key.scancode == SDL_SCANCODE_LEFT || event.key.scancode == SDL_SCANCODE_A) {
				key_state[LEFT] = false;
			}
			else if (event.key.scancode == SDL_SCANCODE_RIGHT || event.key.scancode == SDL_SCANCODE_D) {
				key_state[RIGHT] = false;
			}
		}
	}
}

void GameEngine::update(double elapsedTime) {
	// Rotate
	if (key_state[LEFT]) {
		player.angle -= 5.0f * elapsedTime;
	}
	if (key_state[RIGHT]) {
		player.angle += 5.0f * elapsedTime;
	}

	//Thrust
	if (key_state[UP]) {
		player.dx += sin(player.angle) * 20.0f * elapsedTime;
		player.dy += -cos(player.angle) * 20.0f * elapsedTime;
	}

	// Position update based on velocity
	player.x += player.dx * elapsedTime;
	player.y += player.dy * elapsedTime;

	//std::cout << "ANGLE:" << player.angle << " X: " << player.x << " Y: " << player.y << std::endl;

	for (auto &a : vecAsteroids){
		a.x += a.dx * elapsedTime;
		a.y += a.dy * elapsedTime;
		wrapCoordinates(a.x, a.y, a.x, a.y);
	}
}

void GameEngine::render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	//SDL_SetRenderDrawColor(renderer, 99, 102, 106, SDL_ALPHA_OPAQUE);	// Set window background color dark grey
	SDL_RenderClear(renderer);	// Clear screen with background color

	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // set virtual window background color black
	//SDL_RenderFillRect(renderer, NULL);

	// draw asteroids
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE); // Set color RED
	for (auto &a : vecAsteroids){
		for (int x = 0; x < a.nSize; x++){
			for (int y = 0; y < a.nSize; y++){
				float fx, fy;
				wrapCoordinates(a.x + x, a.y + y, fx, fy);
				SDL_RenderPoint(renderer, fx, fy);
			}
		}
	}

	// Rotate ship
	for (int i = 0; i < 3; i++){
		sx[i] = mx[i] * cosf(player.angle) - my[i] * sinf(player.angle);
		sy[i] = mx[i] * sinf(player.angle) + my[i] * cosf(player.angle);
	}

	// Translate ship
	for (int i = 0; i < 3; i++){
		sx[i] = sx[i] + player.x;
		sy[i] = sy[i] + player.y;
	}

	// Draw ship
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // Set color WHITE
	for (int i = 0; i < 4; i++){
		int j = ( i + 1 );
		float fx1, fy1;
		float fx2, fy2;
		wrapCoordinates(sx[i % 3], sy[i % 3], fx1, fy1);
		wrapCoordinates(sx[j % 3], sy[j % 3], fx2, fy2);
		SDL_RenderLine(renderer, fx1, fy1, fx2, fy2);
	}

	SDL_RenderPresent(renderer);	// Present new screen with drawed objects
}

void GameEngine::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned!" << std::endl;
}

void GameEngine::wrapCoordinates(float ix, float iy, float &ox, float &oy){
	ox = ix;
	oy = iy;
	if ( ix < 0.0f ){
		ox = ix + (float) VIRTUAL_RES_WIDTH;
	}
	if ( ix >= ((float) VIRTUAL_RES_WIDTH )){
		ox = ix - (float) VIRTUAL_RES_WIDTH;
	}
	if ( iy < 0.0f ){
		oy = iy + (float) VIRTUAL_RES_HEIGHT;
	}
	if ( iy >= ((float) VIRTUAL_RES_HEIGHT )){
		oy = iy - (float) VIRTUAL_RES_HEIGHT;
	}
}
