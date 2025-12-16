#include "stdafx.h"
#include "Stroke.h"
#include "Color.h"

Stroke::Stroke() {
	this->strokeWidth = 0;
	this->strokelineCap = "butt";
	this->strokelineJoin = "miter";
	this->miterLimit = 4.0f;
	this->strokedashOffset = 0.0f;
}

Stroke::~Stroke() {}


const color& Stroke::getStrokeColor() const {
	return this->strokeColor;
}

float Stroke::getStrokeWidth() const {
	return this->strokeWidth;
}
string Stroke::getLineCap() const { return strokelineCap; }
string Stroke::getLineJoin() const { return strokelineJoin; }
float Stroke::getMiterLimit() const { return miterLimit; }
float Stroke::getDashOffset() const { return strokedashOffset; }
vector<float> Stroke::getDashArray() const { return strokedashArray; }



void Stroke::setStrokeColor(const color& strokeColor) { this->strokeColor = strokeColor; }
void Stroke::setStrokeWidth(float strokeWidth) { this->strokeWidth = strokeWidth; }
void Stroke::setLineCap(std::string cap) { strokelineCap = cap; }
void Stroke::setLineJoin(std::string join) { strokelineJoin = join; }
void Stroke::setMiterLimit(float limit) { miterLimit = limit; }
void Stroke::setDashArray(const std::vector<float>& arr) { strokedashArray = arr; }
void Stroke::setDashOffset(float offset) { strokedashOffset = offset; }