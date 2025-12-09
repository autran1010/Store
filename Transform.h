#pragma once
#include <Gdiplus.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>

class Transform {
private:
    Gdiplus::Matrix matrix;

    std::vector<float> parseNumbers(const std::string& argsStr);

public:
    Transform();

    // Hàm phân tích cú pháp chuỗi transform SVG (translate, rotate, scale)
    void parseTransform(const std::string& transformValue);

    // Lấy ma trận hiện tại (const)
    const Gdiplus::Matrix& getMatrix() const { return matrix; }

    // Kết hợp ma trận khác vào ma trận hiện tại (cho Group lồng nhau)
    void concatenate(const Gdiplus::Matrix& other);

    // Reset ma trận về ma trận đơn vị
    void reset() { matrix.Reset(); }
};