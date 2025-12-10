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
	bool isFillSet = false;   // <--- THÊM BIẾN NÀY
	bool isStrokeSet = false; // <--- THÊM BIẾN NÀY
public:
	virtual ~Element() = default;
	
	void parse(string, string);
	virtual void parseAttributes(xml_node<>*) = 0;
	virtual void Draw(Gdiplus::Graphics* graphics) = 0;
	virtual void applyParentTransform(const Gdiplus::Matrix& parentMatrix) {
		transform.concatenate(parentMatrix);
	}
	const color& getFill() const;
	void setFill(const color& f);
	const Stroke& getStroke() const;
	void setStroke(const Stroke& s);
	Element() {
		// Constructor nhớ khởi tạo false
		isFillSet = false;
		isStrokeSet = false;
	}
	// ...

	// Thêm getter nếu cần, hoặc để protected cho Group truy cập (nếu Group là friend hoặc kế thừa)
	bool getIsFillSet() const { return isFillSet; }
	bool getIsStrokeSet() const { return isStrokeSet; }
};

#endif

