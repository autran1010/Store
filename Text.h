#include "Point.h"
#include "Element.h"

#ifndef TEXT_H_
#define TEXT_H_

class Text: public Element {
private:
	point position;
	string content;
	float fontsize;
	string font;
	float dx;
	float dy;
	string textAnchor;
	string fontStyle;
	string fontWeight;
public:
	
	Text() : position(0, 0), content(""), fontsize(16), font("Times New Roman"), dx(0), dy(0), textAnchor("start"), fontStyle("normal"), fontWeight("normal") {}
	~Text() override = default;

	void parseAttributes(xml_node<>*) override;

	void Draw(Gdiplus::Graphics* graphics) override;
};

#endif