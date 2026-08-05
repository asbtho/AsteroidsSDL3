#define SDL_MAIN_HANDLED

#include "GameEngine.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 625

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

GameEngine *game = nullptr;
double tp1, tp2;

static void mainloop() {
    if (!game->running()) {
		game->clean();
		#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();  /* this should "kill" the app. */
        #else
        exit(0);
        #endif
	}

	tp2 = SDL_GetTicks();
	double elapsedTime = (tp2 - tp1) / 1000.0;
	tp1 = tp2;

	game->handleEvents();
	game->update(elapsedTime);
	game->render();
}

int main(void) {
    game = new GameEngine();
	game->init("Asteroids", WINDOW_WIDTH, WINDOW_HEIGHT);
	tp1 = SDL_GetTicks();
	
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainloop, 0, 1);
    #else
    while (1) { mainloop(); }
    #endif
}
