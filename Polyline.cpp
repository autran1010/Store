#include "stdafx.h"
#include "Polyline.h"
void polyline::parseAttributes(xml_node<>* node) {
    for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        string name = attr->name();
        string value = attr->value();
        if (name == "points") {
            std::replace(value.begin(), value.end(), ',', ' ');
            stringstream ss(value);
            float x, y;
            while (ss >> x >> y) {
                points.push_back(point(x, y));
            }
        }
        else {
            parse(name, value);
        }
    }
}

void polyline::Draw(Gdiplus::Graphics* graphics) {
    if (points.size() < 2) return;

    GraphicsState state = graphics->Save();

    graphics->MultiplyTransform(this->transform.getMatrix(), MatrixOrderPrepend);

    Gdiplus::Color fillColor(
        (BYTE)(this->fill.getOpacity() * 255.0f),
        (BYTE)(this->fill.getR() * 255.0f),
        (BYTE)(this->fill.getG() * 255.0f),
        (BYTE)(this->fill.getB() * 255.0f));

    Gdiplus::SolidBrush brush(fillColor);

    vector<Gdiplus::PointF> gdiPoints;
    Gdiplus::FillMode mode = (this->fillRule == "evenodd") ? Gdiplus::FillModeAlternate : Gdiplus::FillModeWinding;
    gdiPoints.reserve(points.size());
    for (size_t i = 0; i < points.size(); ++i) {
        gdiPoints.emplace_back(points[i].getX(), points[i].getY());
    }

    if (this->fill.getOpacity() > 0) {
        graphics->FillPolygon(&brush, gdiPoints.data(), (INT)gdiPoints.size(), mode);
    }

    Gdiplus::Pen* pen = this->createPenFromStroke();

    if (pen != nullptr) {
        graphics->DrawLines(pen, gdiPoints.data(), (INT)gdiPoints.size());
        delete pen;
    }

    graphics->Restore(state);
}