#include "stdafx.h"
#include "Color.h"



color::color():r(0), g(0), b(0), opacity(1) {}

color::color(float r, float g, float b, float o) : r(r), g(g), b(b), opacity(o) {}

float color::getR() const { return r; }
float color::getG() const { return g; }
float color::getB() const { return b; }
float color::getOpacity() const { return opacity; }


void color::setR(float val) { r = val; }
void color::setG(float val) { g = val; }
void color::setB(float val) { b = val; }
void color::setOpacity(float val) { opacity = val; }
