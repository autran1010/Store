#include "Color.h"
#include "Stroke.h"
#include "Transform.h"
#include <vector>

#ifndef ELEMENT_H_
#define ELEMENT_H_

class Element {
protected:
    Stroke stroke;
    color fill;
    Transform transform;


    bool isFillSet = false;
    bool isStrokeSet = false;
    bool isStrokeWidthSet = false;
    bool isFillOpacitySet = false;
    bool isStrokeOpacitySet = false;

    color apply(string value);
    //"Mới thêm"
    string fillRule = "nonzero";
    bool isFillRuleSet = false;
    Gdiplus::Pen* createPenFromStroke(); //Hàm mới thêm dùng để pen
public:
    Element() {
        stroke.setStrokeWidth(0.0f);
    }

    Transform& getTransform() { return transform; }
    Stroke& getStroke() { return stroke; }
    color& getFill() { return fill; }
    string getFillRule(const string& c) { return fillRule; }
    void setStroke(const Stroke& s) { stroke = s; }
    void setFill(const color& f) { fill = f; }

    virtual ~Element() = default;
    void parse(string, string);
    virtual void parseAttributes(xml_node<>*) = 0;
    virtual void Draw(Gdiplus::Graphics* graphics) = 0;
    void inheritFrom(Element* parent);
};

#endif