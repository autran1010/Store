#include "Point.h"
#include "Element.h"

#ifndef CIRCLE_H_
#define CIRCLE_H_

class Circle : public Element {
private:
    point center;
    float radius;
public:

    Circle();
    ~Circle() override = default;

    void parseAttributes(xml_node<>*) override;

    void Draw(Gdiplus::Graphics* graphics) override;
};

#endif
