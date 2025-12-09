#include "Color.h"
#include "Stroke.h"
#include "Transform.h"


#ifndef ELEMENT_H_
#define	ELEMENT_H_

class Element {
protected: 
	Stroke stroke;
	color fill;
	Transform transform;
	color apply(string value);
public:
	virtual ~Element() = default;
	
	void parse(string, string);
	virtual void parseAttributes(xml_node<>*) = 0;
	virtual void Draw(Gdiplus::Graphics* graphics) = 0;
	virtual void applyParentTransform(const Gdiplus::Matrix& parentMatrix) {
		transform.concatenate(parentMatrix);
	}
};

#endif

