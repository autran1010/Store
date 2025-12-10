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

    void parseTransform(const std::string& transformValue);

    const Gdiplus::Matrix& getMatrix() const { return matrix; }

    // Kết hợp ma trận khác vào ma trận hiện tại (cho Group lồng nhau)
    void concatenate(const Gdiplus::Matrix& other);

    // Reset ma trận về ma trận đơn vị
    void reset() { matrix.Reset(); }
};