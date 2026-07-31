#pragma once

class Object {
public:
	Object(float x, float y, float dx, float dy, int nSize, float angle);
    Object();
    ~Object();

	//void update();
	//void render();
	//void clean();

    float x;
    float y;
    float dx;
    float dy;
    int nSize;
    float angle;
    
private:
};
