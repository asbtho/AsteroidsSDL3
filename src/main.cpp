#define SDL_MAIN_HANDLED

#include "GameEngine.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 625


GameEngine *game = nullptr;

int main(int argc, char *argv[]) {
	game = new GameEngine();

	game->init("Asteroids", WINDOW_WIDTH, WINDOW_HEIGHT);

	double tp1, tp2;
	tp1 = SDL_GetTicks();

	while (game->running()) {
		tp2 = SDL_GetTicks();
		double elapsedTime = (tp2 - tp1) / 1000.0;
		tp1 = tp2;

		game->handleEvents();
		game->update(elapsedTime);
		game->render();
	}

	game->clean();

	return 0;
}
