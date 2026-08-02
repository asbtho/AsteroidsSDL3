#include "Object.h"

Object::Object(float x, float y, float dx, float dy, int nSize, float angle){
        this->x = x;
        this->y = y;
        this->dx = dx;
        this->dy = dy;
        this->nSize = nSize;
        this->angle = angle;
};

Object::Object(){
};

Object::~Object() {
}
