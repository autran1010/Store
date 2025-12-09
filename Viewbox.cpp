#include "Viewbox.h"


viewbox::viewbox() {
    viewX = 0;
    viewY = 0;
    viewWidth = 800;  
    viewHeight = 600;
    portWidth = 800;
    portHeight = 600;
    rotationAngle = 0.0f; 
}

float viewbox::getViewX() const {
    return this->viewX;
}
float viewbox::getViewY() const {
    return this->viewY;
}
float viewbox::getViewWidth() const {
    return this->viewWidth;
}
float viewbox::getViewHeight() const {
    return this->viewHeight;
}
float viewbox::getPortWidth() const {
    return this->portWidth;
}
float viewbox::getPortHeight() const {
    return this->portHeight;
}
float viewbox::getRotationAngle() const {
    return this->rotationAngle;
}

void viewbox::setViewX(float x) {
    this->viewX = x;
}
void viewbox::setViewY(float y) {
    this->viewY = y;
}
void viewbox::setViewWidth(float w) {
    this->viewWidth = w;
}
void viewbox::setViewHeight(float h) {
    this->viewHeight = h;
}
void viewbox::setPortWidth(float w) {
    this->portWidth = w;
}
void viewbox::setPortHeight(float h) {
    this->portHeight = h;
}
void viewbox::setRotationAngle(float angle) {
    this->rotationAngle = angle;
}