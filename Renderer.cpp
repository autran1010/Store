#include "stdafx.h"
#include "Renderer.h"

#include <gdiplus.h>
using namespace Gdiplus;

Renderer::Renderer() {

}

Renderer::~Renderer() {
    shapes.clear();
}

void Renderer::addShapes(const vector<Element*>& newshape) {
    shapes = newshape;
}


viewbox& Renderer::getViewbox() {
    return this->mainViewbox;
}

void Renderer::pan(float dx, float dy) {
    float scaleX = mainViewbox.getViewWidth() / mainViewbox.getPortWidth();
    float scaleY = mainViewbox.getViewHeight() / mainViewbox.getPortHeight();

    mainViewbox.setViewX(mainViewbox.getViewX() - (dx * scaleX));
    mainViewbox.setViewY(mainViewbox.getViewY() - (dy * scaleY));
}

void Renderer::rotate(float angle) {
    mainViewbox.setRotationAngle(mainViewbox.getRotationAngle() + angle);
}

void Renderer::zoom(float factor) {
    float oldWidth = mainViewbox.getViewWidth();
    float oldHeight = mainViewbox.getViewHeight();

    float newWidth = oldWidth * factor;
    float newHeight = oldHeight * factor;

    mainViewbox.setViewX(mainViewbox.getViewX() + (oldWidth - newWidth) / 2.0f);
    mainViewbox.setViewY(mainViewbox.getViewY() + (oldHeight - newHeight) / 2.0f);

    mainViewbox.setViewWidth(newWidth);
    mainViewbox.setViewHeight(newHeight);
}


void Renderer::renAll(HDC hdc) {
    Gdiplus::Graphics graphics(hdc);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

    float vx = mainViewbox.getViewX();
    float vy = mainViewbox.getViewY();
    float vw = mainViewbox.getViewWidth();
    float vh = mainViewbox.getViewHeight();
    float pw = mainViewbox.getPortWidth();
    float ph = mainViewbox.getPortHeight();
    float angle = mainViewbox.getRotationAngle();

    Gdiplus::Matrix matrix;


    matrix.Translate(pw / 2.0f, ph / 2.0f);
    matrix.Rotate(angle);
    matrix.Translate(-pw / 2.0f, -ph / 2.0f);

    matrix.Scale(pw / vw, ph / vh);


    matrix.Translate(-vx, -vy);

    graphics.SetTransform(&matrix);

    for (Element* shape : shapes) {
        shape->Draw(&graphics);
    }
}

void Renderer::resetView() {
    float pw = mainViewbox.getPortWidth();
    float ph = mainViewbox.getPortHeight();

    mainViewbox.setViewX(0);
    mainViewbox.setViewY(0);
    mainViewbox.setRotationAngle(0);
    mainViewbox.setViewWidth(pw);
    mainViewbox.setViewHeight(ph);
}