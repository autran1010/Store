#include "stdafx.h"
#include "Circle.h"

Circle::Circle() : center(0, 0), radius(0) {}


void Circle::parseAttributes(xml_node<>* node) {
    for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        string name = attr->name();
        string value = attr->value();
        if (name == "cx") center.setX(stof(value));
        else if (name == "cy") center.setY(stof(value));
        else if (name == "r") radius = stof(value);
        else {
            parse(name, value);
        }
    }
}
void Circle::Draw(Gdiplus::Graphics* graphics) {
    Gdiplus::Color fillColor(
        (BYTE)(this->fill.getOpacity() * 255.0f),
        (BYTE)(this->fill.getR() * 255.0f),
        (BYTE)(this->fill.getG() * 255.0f),
        (BYTE)(this->fill.getB() * 255.0f));

    Gdiplus::Color strokeColor(
        (BYTE)(this->stroke.getStrokeColor().getOpacity() * 255.0f),
        (BYTE)(this->stroke.getStrokeColor().getR() *255.0f),
        (BYTE)(this->stroke.getStrokeColor().getG() * 255.0f),
        (BYTE)(this->stroke.getStrokeColor().getB() * 255.0f));


    Gdiplus::SolidBrush brush(fillColor);
    Gdiplus::Pen pen(strokeColor, this->stroke.getStrokeWidth());

    float x = center.getX() - radius;
    float y = center.getY() - radius;
    float w = 2.0f * radius;
    float h = 2.0f * radius;

    if (this->fill.getOpacity() > 0) {
        graphics->FillEllipse(&brush, x, y, w, h);
    }

    if (this->stroke.getStrokeWidth() > 0 && this->stroke.getStrokeColor().getOpacity() > 0) {
        graphics->DrawEllipse(&pen, x, y, w, h);
    }
}