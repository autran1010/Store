#include "stdafx.h"
#include "Point.h"
point::point() {
	x = y = 0;

}

point::point(float cx, float cy)
{
	x = cx;
	y = cy;
}
point::~point() {}

float point::getX() const {
	return this->x;
}

float point::getY() const {
	return this->y;
}


void point::setX(float x) {
	this->x = x;
}

void point::setY(float y) {
	this->y = y;
}

