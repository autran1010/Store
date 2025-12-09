#pragma once
#ifndef PATH_H_
#define PATH_H_

#include "Element.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class Path : public Element {
private:
    std::string d_attr; // Lưu chuỗi lệnh (ví dụ: "M100,200 C...")

public:
    Path();
    ~Path() override = default;

    // Hàm đọc thuộc tính từ XML
    void parseAttributes(xml_node<>* node) override;

    // Hàm vẽ và xử lý các lệnh M, L, H, V, C, Z
    void Draw(Gdiplus::Graphics* graphics) override;
};

#endif