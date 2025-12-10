#include "stdafx.h"
#include "Path.h"
#include <regex>
#include <iostream>

// Hàm chuẩn hóa chuỗi Path: Tách lệnh, xử lý dấu phẩy và dấu trừ dính liền
std::string normalizePathData(std::string d) {
    // 1. Thay thế dấu phẩy bằng khoảng trắng
    std::replace(d.begin(), d.end(), ',', ' ');

    // 2. Thêm khoảng trắng xung quanh các lệnh (M, L, C, Z, s, l...) để tách khỏi số
    static std::regex reCommands("([a-zA-Z])");
    d = std::regex_replace(d, reCommands, " $1 ");

    // 3. Xử lý dấu trừ dính liền (ví dụ: "10-20" -> "10 -20")
    // Logic: Thêm khoảng trắng trước dấu '-' nếu trước đó là số hoặc dấu '.'
    // Tránh trường hợp số mũ khoa học (e-5)
    std::string cleanStr = "";
    for (size_t i = 0; i < d.length(); ++i) {
        if (d[i] == '-' && i > 0) {
            char prev = d[i - 1];
            // Nếu trước dấu trừ là số hoặc dấu chấm, và không phải là 'e' (số mũ)
            if ((isdigit(prev) || prev == '.') && prev != 'e' && prev != 'E') {
                cleanStr += ' ';
            }
        }
        cleanStr += d[i];
    }
    return cleanStr;
}

// Hàm tính điểm phản chiếu cho lệnh S/s (Smooth Bezier)
PointF ReflectPoint(PointF center, PointF point) {
    return PointF(2 * center.X - point.X, 2 * center.Y - point.Y);
}

void Path::parseAttributes(xml_node<>* node) {
    for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
        std::string name = attr->name();
        std::string value = attr->value();

        if (name == "d") {
            // Chuẩn hóa ngay khi đọc
            d_attr = normalizePathData(value);
        }
        else {
            // Gọi hàm parse của lớp cha (Element) để xử lý transform, fill, stroke
            parse(name, value);
        }
    }
}

void Path::Draw(Gdiplus::Graphics* graphics) {
    GraphicsState state = graphics->Save();

    // Áp dụng transform (translate, rotate, scale...) từ lớp cha
    graphics->MultiplyTransform(this->transform.getMatrix(), MatrixOrderPrepend);

    GraphicsPath path;
    stringstream ss(d_attr);

    char command = 0;     // Lệnh hiện tại
    char lastCommand = 0; // Lệnh trước đó (để xử lý phản chiếu S/s)

    // Các biến lưu tọa độ
    float x, y, x1, y1, x2, y2, dx, dy;
    float startX, startY; // Để xử lý lệnh 'H', 'V'

    PointF currentPoint(0, 0);
    PointF startFigurePoint(0, 0); // Điểm bắt đầu của một hình (để đóng path 'Z')
    PointF lastControlPoint(0, 0); // Điểm điều khiển cuối cùng (cho Cubic Bezier)

    // Helper đọc float an toàn
    auto readFloat = [&](float& f) -> bool { return (bool)(ss >> f); };

    // Vòng lặp phân tích cú pháp cải tiến
    // Dùng ss.peek() để kiểm tra ký tự tiếp theo là Lệnh hay Số
    while (ss) {
        // Bỏ qua khoảng trắng thừa
        while (isspace(ss.peek())) {
            ss.get();
        }

        if (ss.eof()) break;

        char nextChar = ss.peek();

        // Nếu là chữ cái, đó là lệnh mới
        if (isalpha(nextChar)) {
            ss >> command;
        }
        // Nếu là số hoặc dấu trừ/chấm, đó là tham số của lệnh CŨ (Implicit command)
        else {
            // Quy tắc SVG: 
            // Nếu lệnh trước là M, các cặp số tiếp theo coi là L
            if (command == 'M') command = 'L';
            else if (command == 'm') command = 'l';
            // Các lệnh khác (L, l, C, c, S, s...) giữ nguyên để lặp lại vẽ
        }

        switch (command) {
        case 'M': // Move Absolute
            if (readFloat(x) && readFloat(y)) {
                path.StartFigure();
                currentPoint = PointF(x, y);
                startFigurePoint = currentPoint;
                lastControlPoint = currentPoint; // Reset control point
            } break;

        case 'm': // Move Relative
            if (readFloat(dx) && readFloat(dy)) {
                path.StartFigure();
                currentPoint = PointF(currentPoint.X + dx, currentPoint.Y + dy);
                startFigurePoint = currentPoint;
                lastControlPoint = currentPoint;
            } break;

        case 'L': // Line Absolute
            if (readFloat(x) && readFloat(y)) {
                path.AddLine(currentPoint, PointF(x, y));
                currentPoint = PointF(x, y);
                lastControlPoint = currentPoint;
            } break;

        case 'l': // Line Relative
            if (readFloat(dx) && readFloat(dy)) {
                PointF p(currentPoint.X + dx, currentPoint.Y + dy);
                path.AddLine(currentPoint, p);
                currentPoint = p;
                lastControlPoint = currentPoint;
            } break;

        case 'H': // Horizontal Line Absolute
            if (readFloat(x)) {
                path.AddLine(currentPoint, PointF(x, currentPoint.Y));
                currentPoint.X = x;
                lastControlPoint = currentPoint;
            } break;

        case 'h': // Horizontal Line Relative
            if (readFloat(dx)) {
                PointF p(currentPoint.X + dx, currentPoint.Y);
                path.AddLine(currentPoint, p);
                currentPoint = p;
                lastControlPoint = currentPoint;
            } break;

        case 'V': // Vertical Line Absolute
            if (readFloat(y)) {
                path.AddLine(currentPoint, PointF(currentPoint.X, y));
                currentPoint.Y = y;
                lastControlPoint = currentPoint;
            } break;

        case 'v': // Vertical Line Relative
            if (readFloat(dy)) {
                PointF p(currentPoint.X, currentPoint.Y + dy);
                path.AddLine(currentPoint, p);
                currentPoint = p;
                lastControlPoint = currentPoint;
            } break;

        case 'C': // Cubic Bezier Absolute
            if (readFloat(x1) && readFloat(y1) && readFloat(x2) && readFloat(y2) && readFloat(x) && readFloat(y)) {
                path.AddBezier(currentPoint, PointF(x1, y1), PointF(x2, y2), PointF(x, y));
                lastControlPoint = PointF(x2, y2);
                currentPoint = PointF(x, y);
            } break;

        case 'c': // Cubic Bezier Relative
            if (readFloat(dx) && readFloat(dy) && readFloat(x2) && readFloat(y2) && readFloat(x) && readFloat(y)) {
                // Lưu ý: Tất cả tham số đều là relative so với currentPoint
                PointF p1(currentPoint.X + dx, currentPoint.Y + dy);
                PointF p2(currentPoint.X + x2, currentPoint.Y + y2);
                PointF pEnd(currentPoint.X + x, currentPoint.Y + y);

                path.AddBezier(currentPoint, p1, p2, pEnd);
                lastControlPoint = p2;
                currentPoint = pEnd;
            } break;

        case 'S': // Smooth Cubic Absolute
        case 's': // Smooth Cubic Relative
        {
            float sx2, sy2, sx, sy;
            if (readFloat(sx2) && readFloat(sy2) && readFloat(sx) && readFloat(sy)) {
                // Tính điểm điều khiển 1 (phản chiếu điểm điều khiển cũ qua điểm hiện tại)
                PointF ctrl1 = currentPoint;

                // Chỉ phản chiếu nếu lệnh trước đó là C, c, S, s
                if (strchr("CcSs", lastCommand)) {
                    ctrl1 = ReflectPoint(currentPoint, lastControlPoint);
                }

                PointF p2, pEnd;
                if (command == 'S') { // Absolute
                    p2 = PointF(sx2, sy2);
                    pEnd = PointF(sx, sy);
                }
                else { // Relative (s)
                    p2 = PointF(currentPoint.X + sx2, currentPoint.Y + sy2);
                    pEnd = PointF(currentPoint.X + sx, currentPoint.Y + sy);
                }

                path.AddBezier(currentPoint, ctrl1, p2, pEnd);
                lastControlPoint = p2;
                currentPoint = pEnd;
            }
        } break;

        case 'Z':
        case 'z':
            path.CloseFigure();
            currentPoint = startFigurePoint;
            lastControlPoint = currentPoint;
            break;

        default:
            // Nếu gặp ký tự lạ không xử lý được, thoát để tránh lặp vô hạn
            ss.setstate(std::ios::failbit);
            break;
        }

        lastCommand = command;
    }

    // --- PHẦN SỬA CHỮA LOGIC VẼ ---

    // 1. Fill (Chỉ vẽ nếu opacity > 0)
    if (fill.getOpacity() > 0.01f) { // So sánh > 0.01 để tránh lỗi float
        Gdiplus::Color fillColor(
            (BYTE)(fill.getOpacity() * 255),
            (BYTE)(fill.getR() * 255),
            (BYTE)(fill.getG() * 255),
            (BYTE)(fill.getB() * 255)
        );
        Gdiplus::SolidBrush brush(fillColor);
        graphics->FillPath(&brush, &path);
    }

    // 2. Stroke (Chỉ vẽ nếu width > 0 VÀ opacity > 0)
    // Trường hợp stroke="none" thì parse sẽ set opacity = 0
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

    graphics->Restore(state);
}