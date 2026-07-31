#include "SDL3/SDL.h"
#include <iostream>
#include <vector>
#include "Object.h"
#include <cmath>

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
	
	Object player;
	float mx[3] = { 0.0f, -2.5f, +2.5f };
	float my[3] = { -5.5f, +2.5f, +2.5f };
	float sx[3], sy[3];

	enum Directions {
		LEFT = 0,
		RIGHT = 1,
		UP = 2
	};
	bool key_state[3] = {false, false, false};
};
