#include "stdafx.h"
#include "Rectangle.h"

rectangle::rectangle() : width(0), height(0), points(0, 0) {}

void rectangle::parseAttributes(xml_node<>* node) {
    for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        string name = attr->name();
        string value = attr->value();

        if (name == "x") points.setX(stof(value));
        else if (name == "y") points.setY(stof(value));
        else if (name == "width") width = stof(value);
        else if (name == "height") height = stof(value);
        else {
            parse(name, value);
        }
    }
}

void rectangle::Draw(Gdiplus::Graphics* graphics) {
    GraphicsState state = graphics->Save();


    graphics->MultiplyTransform(this->transform.getMatrix(), MatrixOrderPrepend);

    Gdiplus::Color fillColor(
        (BYTE)(this->fill.getOpacity() * 255),
        (BYTE)(this->fill.getR() * 255),
        (BYTE)(this->fill.getG() * 255),
        (BYTE)(this->fill.getB() * 255)
    );


    Gdiplus::SolidBrush brush(fillColor);

    if (this->fill.getOpacity() > 0) {
        graphics->FillRectangle(&brush, points.getX(), points.getY(), width, height);
    }

    Gdiplus::Pen* pen = this->createPenFromStroke();

    if (pen != nullptr) {
        graphics->DrawRectangle(pen, points.getX(), points.getY(), width, height);
        delete pen;
    }

    graphics->Restore(state);
}