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

   
    vector<Gdiplus::PointF> gdiPoints;
    gdiPoints.reserve(points.size());
    for (size_t i = 0; i < points.size(); ++i) {
        gdiPoints.emplace_back(points[i].getX(), points[i].getY());
    }

    
    Gdiplus::PointF* pData = gdiPoints.data();
    INT numPoints = (INT)gdiPoints.size();

    
    if (this->fill.getOpacity() > 0) {
        graphics->FillPolygon(&brush, pData, numPoints, Gdiplus::FillModeWinding);
    }
    if (this->stroke.getStrokeWidth() > 0 && this->stroke.getStrokeColor().getOpacity() > 0) {
        graphics->DrawLines(&pen, pData, numPoints);
    }
}