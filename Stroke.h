#include "Color.h"
#ifndef STROKE_H_
#define	STROKE_H_



class Stroke {
private:
	color strokeColor;
	float strokeWidth;
public:
	Stroke();
	~Stroke();

	const color& getStrokeColor() const;
	float getStrokeWidth() const;


	void setStrokeColor(const color&);
	void setStrokeWidth(float);
};

#endif
