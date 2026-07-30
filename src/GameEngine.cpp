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

		window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
		if (window) {
			std::cout << "Window created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, 0);
		if (renderer) {
			SDL_SetRenderLogicalPresentation(renderer, VIRTUAL_RES_WIDTH, VIRTUAL_RES_HEIGHT, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
			std::cout << "Renderer created!" << std::endl;
		}

		isRunning = true;
	} else {
		isRunning = false;
	}

	Object a1(20.0f, 20.0f, 8.0f, -6.0f, 16);
	vecAsteroids.push_back( a1 );
}

void GameEngine::handleEvents() {
	while(SDL_PollEvent(&event)){
		if (event.type == SDL_EVENT_QUIT) {
			isRunning = false;
			break;
		}
		if (event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.scancode == SDL_SCANCODE_UP ||
				event.key.scancode == SDL_SCANCODE_W) {
				//updateShip(UP, true);
			}
			if (event.key.scancode == SDL_SCANCODE_LEFT ||
				event.key.scancode == SDL_SCANCODE_A) {
				//updateShip(LEFT, true);
			}
			else if (event.key.scancode == SDL_SCANCODE_RIGHT ||
				event.key.scancode == SDL_SCANCODE_D) {
				//updateShip(RIGHT, true);
			}
			else if (event.key.scancode == SDL_SCANCODE_ESCAPE){
				isRunning = false;
				break;
			}
		}
		if (event.type == SDL_EVENT_KEY_UP) {
			if (event.key.scancode == SDL_SCANCODE_UP ||
				event.key.scancode == SDL_SCANCODE_W) {
				//updateShip(UP, false);
			}
			if (event.key.scancode == SDL_SCANCODE_LEFT ||
				event.key.scancode == SDL_SCANCODE_RIGHT ||
				event.key.scancode == SDL_SCANCODE_A ||
				event.key.scancode == SDL_SCANCODE_D) {
				//updateShip(LEFT, false);
			}
		}
	}
}

void GameEngine::update(double elapsedTime) {
	for (auto &a : vecAsteroids){
		a.x += a.dx * elapsedTime;
		a.y += a.dy * elapsedTime;
		wrapCoordinates(a.x, a.y, a.x, a.y);
	}
}

void GameEngine::render() {
	SDL_SetRenderDrawColor(renderer, 99, 102, 106, SDL_ALPHA_OPAQUE);	// Set window background color dark grey
	SDL_RenderClear(renderer);	// Clear screen with background color

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // set virtual window background color black
	SDL_RenderFillRect(renderer, NULL);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE); // Set color RED
	for (auto &a : vecAsteroids){
		for (int x = 0; x < a.nSize; x++){
			for (int y = 0; y < a.nSize; y++){
				
				SDL_RenderPoint(renderer, a.x + x, a.y + y);
			}
		}
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
