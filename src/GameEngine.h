#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

#include "Object.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

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

	void wrapCoordinatesPoint(float ix, float iy, float &ox, float &oy);
	void wrapCoordinatesPointInt(int ix, int iy, int &ox, int &oy);
	void drawLine( float x1, float y1, float x2, float y2 );
	void drawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f);
	bool isPointInsideCircle(float cx, float cy, float radius, float x, float y);
	void resetGame();
	void updateScoreText();
	void setWindowTitle(const char* title);

private:
	bool isRunning;
	bool debug = false;
	bool bDead = false;
	int nScore = 0;
	int lastScore = -1;
	SDL_Event event;
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* scoreFont;
	SDL_Texture *scoreTexture;
	SDL_Surface *scoreText;
	SDL_FRect scoreRect;

	std::vector<Object> vecAsteroids;
	std::vector<Object> vecBullets;
	
	Object player;
	std::vector<std::pair<float, float>> vecModelShip;
	std::vector<std::pair<float, float>> vecModelAsteroid;

	enum Directions {
		LEFT = 0,
		RIGHT = 1,
		UP = 2,
		SPACE = 3
	};
	bool key_state[4] = {false, false, false, false};
};
