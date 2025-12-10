#pragma once
#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <string>
#include <gdiplus.h>
#include <vector>
#include <regex>
#include <sstream>

using namespace std;

class Transform {
private:
    Gdiplus::Matrix matrix; // Ma trận biến đổi của GDI+

public:
    Transform();
    ~Transform() = default;

    // Hàm phân tích chuỗi attribute transform="..."
    void parse(string value);

    // Trả về ma trận để Graphics sử dụng
    Gdiplus::Matrix* getMatrix();
};

#endif