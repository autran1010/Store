#include "Element.h"
#include "Point.h"


#ifndef RECTANGLE_H_
#define RECTANGLE_H_

class rectangle : public Element {
private:
	float width;
	float height;
	point points;
public:

	rectangle();

	~rectangle() override = default;

	void parseAttributes(xml_node<>*) override;

	void Draw(Gdiplus::Graphics* graphics) override;
};

#endif
