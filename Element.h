#include "Color.h"
#include "Stroke.h"



#ifndef ELEMENT_H_
#define	ELEMENT_H_

class Element {
protected: 
	Stroke stroke;
	color fill;
	color apply(string value);
public:
	virtual ~Element() = default;
	
	void parse(string, string);
	virtual void parseAttributes(xml_node<>*) = 0;
	virtual void Draw(Gdiplus::Graphics* graphics) = 0;
};

#endif

