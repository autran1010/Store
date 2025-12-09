#include "Point.h"
#include "Element.h"

#ifndef ELLIPSE_H_
#define ELLIPSE_H_

class ellipse : public Element {
private:
	point center;
	float rx, ry;
public:
	
	ellipse() : center(0, 0), rx(0), ry(0) {}
	~ellipse() override = default;
	void parseAttributes(xml_node<>*) override;
	void Draw(Gdiplus::Graphics* graphics) override;
};

#endif 

