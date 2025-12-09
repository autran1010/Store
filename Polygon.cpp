#include "stdafx.h"
#include "Polygon.h"
void polygon::parseAttributes(xml_node<>* node) {
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
void polygon::Draw(Gdiplus::Graphics* graphics) {
    if (points.size() < 3) return;

    Gdiplus::Color fillColor(
        (BYTE)(this->fill.getOpacity() * 255.0f),
        (BYTE)(this->fill.getR() * 255.0f),
        (BYTE)(this->fill.getG() * 255.0f),
        (BYTE)(this->fill.getB() * 255.0f));

    Gdiplus::Color strokeColor(
        (BYTE)(this->stroke.getStrokeColor().getOpacity() * 255.0f),
        (BYTE)(this->stroke.getStrokeColor().getR() * 255.0f),
        (BYTE)(this->stroke.getStrokeColor().getG() * 255.0f),
        (BYTE)(this->stroke.getStrokeColor().getB() * 255.0f));
    
    Gdiplus::SolidBrush brush(fillColor);
    Gdiplus::Pen pen(strokeColor, this->stroke.getStrokeWidth());
    Gdiplus::PointF* gdiPoints = new Gdiplus::PointF[points.size()];
    for (size_t i = 0; i < points.size(); ++i) {
        gdiPoints[i] = Gdiplus::PointF(points[i].getX(), points[i].getY());
    }

    if (this->fill.getOpacity() > 0) {
        graphics->FillPolygon(&brush, gdiPoints, (INT)points.size());
    }
    if (this->stroke.getStrokeWidth() > 0 && this->stroke.getStrokeColor().getOpacity() > 0) {
        graphics->DrawPolygon(&pen, gdiPoints, (INT)points.size());
    }
    delete[] gdiPoints;
}