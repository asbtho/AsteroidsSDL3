#include "SDL3/SDL.h"
#include <iostream>
#include <vector>
#include "Object.h"

class GameEngine {
public:
	GameEngine();
	~GameEngine();

	void init(const char* title, int width, int height);
	void handleEvents();
	void update(double elapsedTime);
	void render();
	void clean();

	bool running() { return isRunning; }

	void wrapCoordinates(float ix, float iy, float &ox, float &oy);
	
private:
	bool isRunning;
	SDL_Event event;
	SDL_Window* window;
	SDL_Renderer* renderer;

	std::vector<Object> vecAsteroids;
};
