
#include "Element.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Ellipse.h"
#include "Line.h"
#include "Polyline.h"
#include "Polygon.h"
#include "Text.h"


#ifndef PARSER_H_
#define	PARSER_H_

class Parser {
private:
	string source;
	vector<Element*>children;
public:
	Parser();
	~Parser();
	Element* createElement(const string&);
	void loadFromFile(const string);
	const vector<Element*>& getChildren();
};

#endif
