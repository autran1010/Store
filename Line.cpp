#include "stdafx.h"
#include "Line.h"

void Line::parseAttributes(xml_node<>* node) {
    for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        string name = attr->name();
        string value = attr->value();
        if (name == "x1") point1.setX(stof(value));
        else if (name == "y1") point1.setY(stof(value));
        else if (name == "x2") point2.setX(stof(value));
        else if (name == "y2") point2.setY(stof(value));
        else {
            parse(name, value);
        }
    }
}
void Line::Draw(Gdiplus::Graphics* graphics) {

    GraphicsState state = graphics->Save();

    graphics->MultiplyTransform(this->transform.getMatrix(), MatrixOrderPrepend);

    Gdiplus::Pen* pen = this->createPenFromStroke();

    if (pen != nullptr) {
        graphics->DrawLine(pen, point1.getX(), point1.getY(), point2.getX(), point2.getY());
        delete pen;
    }
    graphics->Restore(state);
}