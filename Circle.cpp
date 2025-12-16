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

    GraphicsState state = graphics->Save();


    graphics->MultiplyTransform(this->transform.getMatrix(), MatrixOrderPrepend);

    float x = center.getX() - radius;
    float y = center.getY() - radius;
    float w = 2.0f * radius;
    float h = 2.0f * radius;

    Gdiplus::Color fillColor(
        (BYTE)(this->fill.getOpacity() * 255.0f),
        (BYTE)(this->fill.getR() * 255.0f),
        (BYTE)(this->fill.getG() * 255.0f),
        (BYTE)(this->fill.getB() * 255.0f));


    Gdiplus::SolidBrush brush(fillColor);



    if (this->fill.getOpacity() > 0) {
        graphics->FillEllipse(&brush, x, y, w, h);
    }
    Gdiplus::Pen* pen = this->createPenFromStroke();

    if (pen != nullptr) {
        graphics->DrawEllipse(pen, x, y, w, h);
        delete pen;
    }

    // 2. Khôi phục trạng thái
    graphics->Restore(state);
}