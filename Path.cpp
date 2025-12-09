#include "stdafx.h"
#include "Path.h"

Path::Path() : d_attr("") {}

void Path::parseAttributes(xml_node<>* node) {
    for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        std::string name = attr->name();
        std::string value = attr->value();

        if (name == "d") {
            d_attr = value;
        }
        else {
            parse(name, value);
        }
    }
}

void Path::Draw(Gdiplus::Graphics* graphics) {
    if (d_attr.empty()) return;

    // 1. Transform (Yêu cầu 1)
    Gdiplus::GraphicsState state = graphics->Save();
    graphics->MultiplyTransform(&this->transform.getMatrix());

    // 2. Màu sắc
    Gdiplus::Color fillColor(
        (BYTE)(this->fill.getOpacity() * 255.0f), (BYTE)(this->fill.getR() * 255.0f),
        (BYTE)(this->fill.getG() * 255.0f), (BYTE)(this->fill.getB() * 255.0f));
    Gdiplus::Color strokeColor(
        (BYTE)(this->stroke.getStrokeColor().getOpacity() * 255.0f), (BYTE)(this->stroke.getStrokeColor().getR() * 255.0f),
        (BYTE)(this->stroke.getStrokeColor().getG() * 255.0f), (BYTE)(this->stroke.getStrokeColor().getB() * 255.0f));

    Gdiplus::SolidBrush brush(fillColor);
    Gdiplus::Pen pen(strokeColor, this->stroke.getStrokeWidth());

    // 3. Xử lý Path
    Gdiplus::GraphicsPath path(Gdiplus::FillModeWinding);

    // Làm sạch chuỗi: thêm khoảng trắng quanh lệnh và thay phẩy bằng khoảng trắng
    std::string clean_d = "";
    for (char c : d_attr) {
        if (isalpha(c)) { clean_d += " "; clean_d += c; clean_d += " "; }
        else if (c == ',') { clean_d += " "; }
        else { clean_d += c; }
    }

    std::stringstream ss(clean_d);
    std::string token;

    char command = ' ';
    float currentX = 0, currentY = 0;
    float startX = 0, startY = 0;

    while (ss >> token) {
        // Nếu là lệnh (chữ cái)
        if (isalpha(token[0])) {
            command = token[0];

            // Xử lý ngay lệnh Z (ClosePath) [cite: 35]
            if (command == 'Z' || command == 'z') {
                path.CloseFigure();
                currentX = startX;
                currentY = startY;
            }
            // M, L, H, V, C cần tham số nên sẽ xử lý ở vòng lặp sau
            continue;
        }

        // Nếu là số, thực hiện lệnh tương ứng
        try {
            if (command == 'M' || command == 'm') { // MoveTo [cite: 31]
                float x = std::stof(token);
                float y; ss >> y;
                path.StartFigure();
                currentX = x; currentY = y;
                startX = x; startY = y;
            }
            else if (command == 'L' || command == 'l') { // LineTo [cite: 32]
                float x = std::stof(token);
                float y; ss >> y;
                path.AddLine(currentX, currentY, x, y);
                currentX = x; currentY = y;
            }
            else if (command == 'H' || command == 'h') { // Horizontal [cite: 34]
                float x = std::stof(token);
                path.AddLine(currentX, currentY, x, currentY);
                currentX = x;
            }
            else if (command == 'V' || command == 'v') { // Vertical [cite: 34]
                float y = std::stof(token);
                path.AddLine(currentX, currentY, currentX, y);
                currentY = y;
            }
            else if (command == 'C' || command == 'c') { // Cubic Bezier [cite: 36]
                float x1 = std::stof(token);
                float y1, x2, y2, x, y;
                ss >> y1 >> x2 >> y2 >> x >> y;
                path.AddBezier(currentX, currentY, x1, y1, x2, y2, x, y);
                currentX = x; currentY = y;
            }
        }
        catch (...) {}
    }

    // 4. Vẽ
    if (this->fill.getOpacity() > 0) graphics->FillPath(&brush, &path);
    if (this->stroke.getStrokeWidth() > 0) graphics->DrawPath(&pen, &path);

    graphics->Restore(state);
}