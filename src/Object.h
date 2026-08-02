#pragma once

class Object {
public:
	Object(float x, float y, float dx, float dy, int nSize, float angle);
    Object();
    ~Object();

    float x;
    float y;
    float dx;
    float dy;
    int nSize;
    float angle;
    
private:
};
