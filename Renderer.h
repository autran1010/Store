#pragma once
#include "stdafx.h"
#include <vector>
#include "Element.h" 
#include "viewbox.h" 

using std::vector;

class Renderer {
private:
    vector<Element*> shapes;
    viewbox mainViewbox;

public:
    Renderer();
    ~Renderer();

    void addShapes(const vector<Element*>& newshape);
    void renAll(HDC hdc);
    void pan(float dx, float dy);
    void zoom(float factor);
    void rotate(float angle);
    viewbox& getViewbox();
    void resetView();
};