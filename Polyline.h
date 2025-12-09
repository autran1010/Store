#include "Point.h"
#include "Element.h"
#ifndef POLYLINE_H_
#define POLYLINE_H_

class polyline : public Element {
private:
	vector<point> points;
public:
	polyline() = default;
	~polyline() override = default;
	void parseAttributes(xml_node<>*) override;
	void Draw(Gdiplus::Graphics* graphics) override;
};

#endif
