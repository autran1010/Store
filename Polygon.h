#include "Point.h"
#include "Element.h"
#ifndef POLYGON_H_
#define POLYGON_H_

class polygon : public Element {
private:
	vector<point> points;
public:
	polygon() = default;
	~polygon() override = default;
	void parseAttributes(xml_node<>*) override;
	void Draw(Gdiplus::Graphics* graphics) override;
};

#endif
