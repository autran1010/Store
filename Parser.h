#pragma once
#ifndef PARSER_H_
#define PARSER_H_

#include "Element.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Line.h"
#include "Polyline.h"
#include "Polygon.h"
#include "Text.h"
#include "Path.h"  // Yêu cầu 2
#include "Group.h" // Yêu cầu 3

#include <vector>
#include <string>

class Parser {
private:
    std::string source;
    std::vector<Element*> children;

    // [QUAN TRỌNG] Hàm đệ quy để parse group lồng nhau
    Element* parseElement(xml_node<>* node);

public:
    Parser();
    ~Parser();

    Element* createElement(const std::string& name);
    void loadFromFile(const std::string source);
    const std::vector<Element*>& getChildren();
};

#endif