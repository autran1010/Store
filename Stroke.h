#include "Color.h"
#ifndef STROKE_H_
#define	STROKE_H_
class Stroke {
private:
	color strokeColor;
	float strokeWidth;

	//Mới thêm
	string strokelineCap;
	string strokelineJoin;
	float miterLimit;
	vector<float> strokedashArray;
	float strokedashOffset;

public:
	Stroke();
	~Stroke();

	const color& getStrokeColor() const;
	float getStrokeWidth() const;

	string getLineCap() const;
	string getLineJoin() const;
	float getMiterLimit() const;
	float getDashOffset() const;
	vector<float> getDashArray() const;


	void setStrokeColor(const color&);
	void setStrokeWidth(float);
	void setLineCap(string cap);
	void setLineJoin(string join);
	void setMiterLimit(float limit);
	void setDashArray(const std::vector<float>& arr);
	void setDashOffset(float offset);
};

#endif