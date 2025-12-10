#include "stdafx.h"
#include "Transform.h"

Transform::Transform() {
    // Matrix mặc định là ma trận đơn vị (Identity)
    matrix.Reset();
}

Gdiplus::Matrix* Transform::getMatrix() {
    return &matrix;
}

void Transform::parse(string value) {
    // Regex để bắt các cụm lệnh: tên_lệnh(tham_số)
    // Ví dụ: translate(10, 20) hoặc rotate(45)
    regex re("([a-z]+)\\s*\\(([^)]+)\\)");
    sregex_iterator next(value.begin(), value.end(), re);
    sregex_iterator end;

    while (next != end) {
        smatch match = *next;
        string type = match.str(1);   // Tên lệnh: translate, rotate, scale
        string params = match.str(2); // Tham số: "10, 20"

        // Thay thế dấu phẩy bằng khoảng trắng để dễ đọc bằng stringstream
        replace(params.begin(), params.end(), ',', ' ');
        stringstream ss(params);
        float val1, val2;

        if (type == "translate") {
            ss >> val1;
            if (!(ss >> val2)) val2 = 0; // Nếu thiếu y, mặc định y = 0
            matrix.Translate(val1, val2, Gdiplus::MatrixOrderPrepend);
        }
        else if (type == "rotate") {
            ss >> val1;
            // SVG rotate mặc định xoay quanh gốc (0,0) hiện tại
            matrix.Rotate(val1, Gdiplus::MatrixOrderPrepend);
        }
        else if (type == "scale") {
            ss >> val1;
            if (!(ss >> val2)) val2 = val1; // Nếu thiếu sy, mặc định sy = sx
            matrix.Scale(val1, val2, Gdiplus::MatrixOrderPrepend);
        }

        next++;
    }
}