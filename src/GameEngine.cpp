#include "GameEngine.h"

#define VIRTUAL_RES_WIDTH 160
#define VIRTUAL_RES_HEIGHT 100

GameEngine::GameEngine() {
}

GameEngine::~GameEngine() {
}

void GameEngine::init(const char* title, int width, int height) {
	if (SDL_Init(SDL_INIT_AUDIO || SDL_INIT_VIDEO || SDL_INIT_EVENTS) == 1 && TTF_Init() == 1) {
		std::cout << "Subsystems initialized!..." << std::endl;

		window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
		if (window) {
			std::cout << "Window created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, 0);
		if (renderer) {
			SDL_SetRenderLogicalPresentation(renderer, VIRTUAL_RES_WIDTH, VIRTUAL_RES_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
			std::cout << "Renderer created!" << std::endl;
		}

		scoreFont = TTF_OpenFont("ARCADECLASSIC.ttf", 13.0f);

		isRunning = true;
	} else {
		isRunning = false;
	}

	// Set ship vector coordinates
	vecModelShip = {
		{ 0.0f, -4.0f },
		{ -2.0f, +2.0f },
		{ +2.0f, +2.0f }
	};

	// Generate asteroid model coordinates (circle)
	int verts = 20;
	for (int i = 0; i < verts; i++){
		float radius = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f; 
		float a = ((float)i / (float)verts) * 6.28318f;
		vecModelAsteroid.push_back(std::make_pair(sinf(a) * radius, cosf(a) * radius));
	}

	// Init game variables
	updateScoreText();
	resetGame();
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
			if (event.key.scancode == SDL_SCANCODE_SPACE) {
				key_state[SPACE] = true;
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
			if (event.key.scancode == SDL_SCANCODE_SPACE) {
				if ( key_state[SPACE] == true ) {
					// Fire bullet
					Object b(player.x + (5.0f * sin(player.angle)), player.y - (5.0f * cos(player.angle)), 50.0f * sin(player.angle), -50.0f * cos(player.angle), 0, 0.0f);
					vecBullets.push_back(b);
				}
				key_state[SPACE] = false;
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
	if (bDead) {
		resetGame();
	}
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
	wrapCoordinatesPoint(player.x, player.y, player.x, player.y);

	// Check ship collision with asteroids
	for (auto &a : vecAsteroids){
		if (isPointInsideCircle(a.x, a.y, a.nSize, player.x, player.y)) {
			bDead = true;
		}
	}

	for (auto &a : vecAsteroids){
		a.x += a.dx * elapsedTime;
		a.y += a.dy * elapsedTime;
		a.angle += 0.5f * elapsedTime;
		wrapCoordinatesPoint(a.x, a.y, a.x, a.y);
	}

	std::vector<Object> newAsteroids;

	for (auto &b : vecBullets){
		b.x += b.dx * elapsedTime;
		b.y += b.dy * elapsedTime;
		wrapCoordinatesPoint(b.x, b.y, b.x, b.y);

		// Check for collision with asteroids
		for (auto &a : vecAsteroids){
			if (isPointInsideCircle(a.x, a.y, a.nSize, b.x, b.y)) {
				b.x = -100.0f; // Mark bullet for removal

				// Split asteroid into two smaller ones if size is greater than 4
				if (a.nSize > 4) {
					float angle1 = ((float)rand() / (float)RAND_MAX) * 6.28318f;
					float angle2 = ((float)rand() / (float)RAND_MAX) * 6.28318f;
					Object a1(a.x, a.y, 10.0f * sin(angle1), -10.0f * cos(angle1), (int)a.nSize >> 1, 0.0f);
					Object a2(a.x, a.y, 10.0f * sin(angle2), -10.0f * cos(angle2), (int)a.nSize >> 1, 0.0f);
					newAsteroids.push_back(a1);
					newAsteroids.push_back(a2);
				}

				a.x = -100.0f; // Mark asteroid for removal
				nScore += 100; // Increase score
			}
		}
	}

	//Append new asteroids to the existing vector
	for (auto a : newAsteroids){
		vecAsteroids.push_back(a);
	}

	// Remove off-screen bullets
	if (vecBullets.size() > 0) {
		auto i = remove_if(vecBullets.begin(), vecBullets.end(), [&](Object o) { return (o.x < 1 || o.y < 1 || o.x >= VIRTUAL_RES_WIDTH - 1 || o.y >= VIRTUAL_RES_HEIGHT - 1); });
		if ( i != vecBullets.end() ) {
			vecBullets.erase(i);
		}
	}

	// Remove off-screen asteroids
	if (vecAsteroids.size() > 0) {
		auto i = remove_if(vecAsteroids.begin(), vecAsteroids.end(), [&](Object o) { return (o.x < 0); });
		if ( i != vecAsteroids.end() ) {
			vecAsteroids.erase(i);
		}
	}

	if (vecAsteroids.empty()) {
		nScore += 1000;
		Object newAsteroid(30.0f * sinf(player.angle - 3.14159f / 2.0f), 30.0f * cosf(player.angle - 3.14159f / 2.0f), 10.0f * sinf(player.angle), -10.0f * cosf(player.angle), (int)16, 0.0f);
		Object newAsteroid2(30.0f * sinf(player.angle + 3.14159f / 2.0f), 30.0f * cosf(player.angle + 3.14159f / 2.0f), 10.0f * sinf(-player.angle), -10.0f * cosf(-player.angle), (int)16, 0.0f);
		vecAsteroids.push_back(newAsteroid);
		vecAsteroids.push_back(newAsteroid2);
	}

	updateScoreText();

	if (debug) {
		//std::cout << "P_ANGLE:" << player.angle << " P_X: " << player.x << " P_Y: " << player.y << std::endl;
	}
}

void GameEngine::render() {
	SDL_SetRenderDrawColor(renderer, 99, 102, 106, SDL_ALPHA_OPAQUE);	// Set window background color dark grey
	SDL_RenderClear(renderer);	// Clear screen with background color

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // set virtual window background color black
	SDL_RenderFillRect(renderer, NULL);

	// Draw Score
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // WHITE
	SDL_RenderTexture(renderer, scoreTexture, NULL, &scoreRect);

	// Draw asteroids
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE); // YELLOW
	for (auto &a : vecAsteroids){
		drawWireFrameModel(vecModelAsteroid, a.x, a.y, a.angle, a.nSize);
	}

	// Draw bullets
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // WHITE
	for (auto &b : vecBullets){
		SDL_RenderPoint(renderer, b.x, b.y);
	}

	// Draw ship
	drawWireFrameModel(vecModelShip, player.x, player.y, player.angle, 1.0f);

	SDL_RenderPresent(renderer);	// Present new screen with drawed objects
}

void GameEngine::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_CloseFont(scoreFont);
	TTF_Quit();
	SDL_Quit();
	std::cout << "Game cleaned!" << std::endl;
}

void GameEngine::wrapCoordinatesPoint(float ix, float iy, float &ox, float &oy){
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

void GameEngine::wrapCoordinatesPointInt(int ix, int iy, int &ox, int &oy){
	ox = ix;
	oy = iy;
	if ( ix < 0 ){
		ox = ix + VIRTUAL_RES_WIDTH;
	}
	if ( ix >= VIRTUAL_RES_WIDTH){
		ox = ix - VIRTUAL_RES_WIDTH;
	}
	if ( iy < 0 ){
		oy = iy + VIRTUAL_RES_HEIGHT;
	}
	if ( iy >= VIRTUAL_RES_HEIGHT ){
		oy = iy - VIRTUAL_RES_HEIGHT;
	}
}

void GameEngine::drawLine( float x1, float y1, float x2, float y2 ){
	// Bresenham's line algorithm
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if(steep){
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if(x1 > x2){
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;

	for(int x=(int)x1; x<=maxX; x++){
		if(steep){
			int fx, fy;
			wrapCoordinatesPointInt(y, x, fx, fy); // x y switched because of steep
			SDL_RenderPoint(renderer, fx, fy);
		} else {
			int fx, fy;
			wrapCoordinatesPointInt(x, y, fx, fy);
			SDL_RenderPoint(renderer, fx, fy);
		}
				
		error -= dy;
		if(error < 0){
			y += ystep;
			error += dx;
		}
	}
}

void GameEngine::drawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y, float r, float s){
	// pair.first = x coordinate
	// pair.second = y coordinate
	
	// Create translated model vector of coordinate pairs
	std::vector<std::pair<float, float>> vecTransformedCoordinates;
	int verts = vecModelCoordinates.size();
	vecTransformedCoordinates.resize(verts);

	// Rotate
	for (int i = 0; i < verts; i++){
		vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
		vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
	}

	// Scale
	for (int i = 0; i < verts; i++){
		vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
		vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
	}

	// Translate
	for (int i = 0; i < verts; i++){
		vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
		vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
	}

	// Draw Closed Polygon
	for (int i = 0; i < verts + 1; i++){
		int j = (i + 1);
		drawLine(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second); // bresenham integer based, retro style with better edge wrapping
		//SDL_RenderLine(renderer, vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second);
	}
}

bool GameEngine::isPointInsideCircle(float cx, float cy, float radius, float x, float y) {
	return sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) < radius;
}

void GameEngine::resetGame() {
	vecAsteroids.clear();
	vecBullets.clear();	

	// Initialize asteroids
	Object a1(20.0f, 20.0f, 8.0f, -6.0f, (int)16, 0.0f);
	Object a2(100.0f, 20.0f, -5.0f, -6.0f, (int)16, 0.0f);
	vecAsteroids.push_back( a1 );
	vecAsteroids.push_back( a2 );

	//Initialize player position, velocity and angle
	player.x = VIRTUAL_RES_WIDTH / 2.0f;
	player.y = VIRTUAL_RES_HEIGHT / 2.0f;
	player.dx = 0.0f;
	player.dy = 0.0f;
	player.angle = 0.0f;

	bDead = false;
	nScore = 0;
	lastScore = -1;
}


void GameEngine::updateScoreText(){
	if (nScore == lastScore) {
    	return;
    }
    lastScore = nScore;

	if (scoreTexture) {
        SDL_DestroyTexture(scoreTexture);
        scoreTexture = NULL;
    }

	std::string str = "Score   " + std::to_string(nScore);
    const char* scoreChar = str.c_str();
	scoreText = TTF_RenderText_Solid(scoreFont, scoreChar, 0, {255, 255, 255, SDL_ALPHA_OPAQUE});
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreText);
	SDL_SetTextureScaleMode(scoreTexture, SDL_SCALEMODE_NEAREST);
	scoreRect = { 1.0f, 1.0f, (float)scoreTexture->w, (float)scoreTexture->h };
	SDL_DestroySurface(scoreText);
}
