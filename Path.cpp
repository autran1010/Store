#include "stdafx.h"
#include "Path.h"
#include <regex>
#include <iostream>

std::string normalizePathData(std::string d) {
    std::replace(d.begin(), d.end(), ',', ' ');
    static std::regex reCommands("([a-zA-Z])");
    d = std::regex_replace(d, reCommands, " $1 ");
    std::string cleanStr = "";
    for (size_t i = 0; i < d.length(); ++i) {
        if (d[i] == '-' && i > 0) {
            char prev = d[i - 1];
            if ((isdigit(prev) || prev == '.') && prev != 'e' && prev != 'E') {
                cleanStr += ' ';
            }
        }
        cleanStr += d[i];
    }
    return cleanStr;
}

PointF ReflectPoint(PointF center, PointF point) {
    return PointF(2 * center.X - point.X, 2 * center.Y - point.Y);
}

void Path::parseAttributes(xml_node<>* node) {
    for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        std::string name = attr->name();
        std::string value = attr->value();

        if (name == "d") {
            d_attr = normalizePathData(value);
        }
        else {
            parse(name, value);
        }
    }
}

void Path::Draw(Gdiplus::Graphics* graphics) {
    GraphicsState state = graphics->Save();
    graphics->MultiplyTransform(this->transform.getMatrix(), MatrixOrderPrepend);

    GraphicsPath path;
    if (this->fillRule == "evenodd") {
        path.SetFillMode(Gdiplus::FillModeAlternate);
    }
    else {
        path.SetFillMode(Gdiplus::FillModeWinding);
    }
    stringstream ss(d_attr);

    char command = 0;
    char lastCommand = 0;

    float x, y, x1, y1, x2, y2, dx, dy;
    float startX, startY;

    PointF currentPoint(0, 0);
    PointF startFigurePoint(0, 0);
    PointF lastControlPoint(0, 0);
    PointF lastQuadControlPoint(0, 0);

    auto readFloat = [&](float& f) -> bool { return (bool)(ss >> f); };

    while (ss) {
        while (isspace(ss.peek())) {
            ss.get();
        }

        if (ss.eof()) break;

        char nextChar = ss.peek();


        if (isalpha(nextChar)) {
            ss >> command;
        }

        else {

            if (command == 'M') command = 'L';
            else if (command == 'm') command = 'l';

        }

        switch (command) {
        case 'M':
            if (readFloat(x) && readFloat(y)) {
                path.StartFigure();
                currentPoint = PointF(x, y);
                startFigurePoint = currentPoint;
                lastControlPoint = currentPoint;
            } break;

        case 'm':
            if (readFloat(dx) && readFloat(dy)) {
                path.StartFigure();
                currentPoint = PointF(currentPoint.X + dx, currentPoint.Y + dy);
                startFigurePoint = currentPoint;
                lastControlPoint = currentPoint;
            } break;

        case 'L':
            if (readFloat(x) && readFloat(y)) {
                path.AddLine(currentPoint, PointF(x, y));
                currentPoint = PointF(x, y);
                lastControlPoint = currentPoint;
            } break;

        case 'l':
            if (readFloat(dx) && readFloat(dy)) {
                PointF p(currentPoint.X + dx, currentPoint.Y + dy);
                path.AddLine(currentPoint, p);
                currentPoint = p;
                lastControlPoint = currentPoint;
            } break;

        case 'H':
            if (readFloat(x)) {
                path.AddLine(currentPoint, PointF(x, currentPoint.Y));
                currentPoint.X = x;
                lastControlPoint = currentPoint;
            } break;

        case 'h':
            if (readFloat(dx)) {
                PointF p(currentPoint.X + dx, currentPoint.Y);
                path.AddLine(currentPoint, p);
                currentPoint = p;
                lastControlPoint = currentPoint;
            } break;

        case 'V':
            if (readFloat(y)) {
                path.AddLine(currentPoint, PointF(currentPoint.X, y));
                currentPoint.Y = y;
                lastControlPoint = currentPoint;
            } break;

        case 'v':
            if (readFloat(dy)) {
                PointF p(currentPoint.X, currentPoint.Y + dy);
                path.AddLine(currentPoint, p);
                currentPoint = p;
                lastControlPoint = currentPoint;
            } break;

        case 'C':
            if (readFloat(x1) && readFloat(y1) && readFloat(x2) && readFloat(y2) && readFloat(x) && readFloat(y)) {
                path.AddBezier(currentPoint, PointF(x1, y1), PointF(x2, y2), PointF(x, y));
                lastControlPoint = PointF(x2, y2);
                currentPoint = PointF(x, y);
            } break;

        case 'c':
            if (readFloat(dx) && readFloat(dy) && readFloat(x2) && readFloat(y2) && readFloat(x) && readFloat(y)) {

                PointF p1(currentPoint.X + dx, currentPoint.Y + dy);
                PointF p2(currentPoint.X + x2, currentPoint.Y + y2);
                PointF pEnd(currentPoint.X + x, currentPoint.Y + y);

                path.AddBezier(currentPoint, p1, p2, pEnd);
                lastControlPoint = p2;
                currentPoint = pEnd;
            } break;

        case 'S': case 's':
        {
            float sx2, sy2, sx, sy;
            if (readFloat(sx2) && readFloat(sy2) && readFloat(sx) && readFloat(sy)) {

                PointF ctrl1 = currentPoint;


                if (strchr("CcSs", lastCommand)) {
                    ctrl1 = ReflectPoint(currentPoint, lastControlPoint);
                }

                PointF p2, pEnd;
                if (command == 'S') {
                    p2 = PointF(sx2, sy2);
                    pEnd = PointF(sx, sy);
                }
                else {
                    p2 = PointF(currentPoint.X + sx2, currentPoint.Y + sy2);
                    pEnd = PointF(currentPoint.X + sx, currentPoint.Y + sy);
                }

                path.AddBezier(currentPoint, ctrl1, p2, pEnd);
                lastControlPoint = p2;
                currentPoint = pEnd;
            }
        } break;

        case 'Q': case 'q':
        {
            float qx1, qy1, qx, qy;
            if (readFloat(qx1) && readFloat(qy1) && readFloat(qx) && readFloat(qy)) {
                PointF p1, pEnd;

                if (command == 'q') {
                    p1 = PointF(currentPoint.X + qx1, currentPoint.Y + qy1);
                    pEnd = PointF(currentPoint.X + qx, currentPoint.Y + qy);
                }
                else {
                    p1 = PointF(qx1, qy1);
                    pEnd = PointF(qx, qy);
                }

                PointF c1(currentPoint.X + (2.0f / 3.0f) * (p1.X - currentPoint.X),
                    currentPoint.Y + (2.0f / 3.0f) * (p1.Y - currentPoint.Y));
                PointF c2(pEnd.X + (2.0f / 3.0f) * (p1.X - pEnd.X),
                    pEnd.Y + (2.0f / 3.0f) * (p1.Y - pEnd.Y));

                path.AddBezier(currentPoint, c1, c2, pEnd);

                lastQuadControlPoint = p1;
                lastControlPoint = c2;
                currentPoint = pEnd;
            }
        } break;

        case 'T': case 't':
        {
            float qx, qy;
            if (readFloat(qx) && readFloat(qy)) {
                PointF p1 = currentPoint;

                if (strchr("QqTt", lastCommand)) {
                    p1 = ReflectPoint(currentPoint, lastQuadControlPoint);
                }

                PointF pEnd;
                if (command == 't') {
                    pEnd = PointF(currentPoint.X + qx, currentPoint.Y + qy);
                }
                else {
                    pEnd = PointF(qx, qy);
                }

                PointF c1(currentPoint.X + (2.0f / 3.0f) * (p1.X - currentPoint.X),
                    currentPoint.Y + (2.0f / 3.0f) * (p1.Y - currentPoint.Y));
                PointF c2(pEnd.X + (2.0f / 3.0f) * (p1.X - pEnd.X),
                    pEnd.Y + (2.0f / 3.0f) * (p1.Y - pEnd.Y));

                path.AddBezier(currentPoint, c1, c2, pEnd);

                lastQuadControlPoint = p1;
                lastControlPoint = c2;
                currentPoint = pEnd;
            }
        } break;
        case 'Z': case 'z':
            path.CloseFigure();
            currentPoint = startFigurePoint;
            lastControlPoint = currentPoint;
            break;

        default:
            ss.setstate(std::ios::failbit);
            break;
        }

        lastCommand = command;
    }


    if (fill.getOpacity() > 0.01f) {
        Gdiplus::Color fillColor(
            (BYTE)(fill.getOpacity() * 255),
            (BYTE)(fill.getR() * 255),
            (BYTE)(fill.getG() * 255),
            (BYTE)(fill.getB() * 255)
        );
        Gdiplus::SolidBrush brush(fillColor);
        graphics->FillPath(&brush, &path);
    }
    if (stroke.getStrokeWidth() > 0.0f && stroke.getStrokeColor().getOpacity() > 0.01f) {
        Gdiplus::Color strokeColor(
            (BYTE)(stroke.getStrokeColor().getOpacity() * 255),
            (BYTE)(stroke.getStrokeColor().getR() * 255),
            (BYTE)(stroke.getStrokeColor().getG() * 255),
            (BYTE)(stroke.getStrokeColor().getB() * 255)
        );
        Gdiplus::Pen pen(strokeColor, stroke.getStrokeWidth());
        graphics->DrawPath(&pen, &path);
    }

    Gdiplus::Pen* pen = this->createPenFromStroke();

    if (pen != nullptr) {
        graphics->DrawPath(pen, &path);
        delete pen;
    }

    graphics->Restore(state);
}