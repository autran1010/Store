#include "Element.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Ellipse.h"
#include "Line.h"
#include "Polyline.h"
#include "Polygon.h"
#include "Text.h"
// Thêm 2 header mới
#include "Path.h"
#include "Group.h" 

#ifndef PARSER_H_
#define	PARSER_H_

class Parser {
private:
	string source;
	vector<Element*> children;

	// --- CẬP NHẬT DÒNG NÀY ---
	// Thêm tham số mặc định parentStyle = nullptr
	void parseRecursive(xml_node<>* node, vector<Element*>& container, Element* parentStyle = nullptr);

public:
	Parser();
	~Parser();
	Element* createElement(const string&);
	void loadFromFile(const string);
	const vector<Element*>& getChildren();
};

#endif