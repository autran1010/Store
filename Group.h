#pragma once
#ifndef GROUP_H_
#define GROUP_H_

#include "Element.h"
#include <vector>

class Group : public Element {
private:
    std::vector<Element*> children; // Danh sách con

public:
    Group();
    ~Group() override;

    void parseAttributes(xml_node<>* node) override;
    void Draw(Gdiplus::Graphics* graphics) override;

    void addChild(Element* child); // Hàm thêm con
};

#endif