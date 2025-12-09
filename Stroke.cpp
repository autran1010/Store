#include "stdafx.h"
#include "Stroke.h"
#include "Color.h"

Stroke::Stroke() {
	this->strokeWidth = 0;
}

Stroke::~Stroke() {
	this->strokeWidth = 0;
}


const color& Stroke::getStrokeColor() const {
	return this->strokeColor;
}

float Stroke::getStrokeWidth() const {
	return this->strokeWidth;
}


void Stroke::setStrokeColor(const color& strokeColor) {
	this->strokeColor = strokeColor;
}

void Stroke::setStrokeWidth(float strokeWidth) {
	this->strokeWidth = strokeWidth;
}
