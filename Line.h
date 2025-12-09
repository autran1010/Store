
#include "Point.h"
#include "Element.h"
#ifndef LINE_H_
#define LINE_H_

class Line : public Element {
private:
	point point1, point2;
public:
	Line() : point1(0, 0), point2(0,0) {}
	~Line() override = default;
	void parseAttributes(xml_node<>*) override;
	void Draw(Gdiplus::Graphics* graphics) override;
};

#endif