#define SDL_MAIN_HANDLED

#include "GameEngine.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 625


GameEngine *game = nullptr;

int main(int argc, char *argv[]) {
	game = new GameEngine();

	game->init("Asteroids - # FPS", WINDOW_WIDTH, WINDOW_HEIGHT);

	double tp1, tp2;
	double fpscounterLast, fpscounter;
	Uint32 frame_count = 0;
	char title[64];
	fpscounterLast = tp1 = SDL_GetTicks();

	while (game->running()) {
		frame_count++;
		fpscounter = tp2 = SDL_GetTicks();
		double elapsedTime = (tp2 - tp1) / 1000.0;
		tp1 = tp2;

		if (fpscounter - fpscounterLast >= 1000) {
			float fps = frame_count / ((fpscounter - fpscounterLast) / 1000.0f);
			SDL_snprintf(title, sizeof(title), "Asteroids - %.1f FPS", fps);
			game->setWindowTitle(title);
			frame_count = 0;
			fpscounterLast = fpscounter;
		}

		game->handleEvents();
		game->update(elapsedTime);
		game->render();
	}

	game->clean();

	return 0;
}
