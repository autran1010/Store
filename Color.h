

#ifndef _COLOR_H_
#define _COLOR_H_
class color
{
private:
	float r, g, b, opacity;
public:
	color();
	color(float r, float g, float b, float o);

	float getR() const;
	float getG() const;
	float getB() const;
	float getOpacity() const;

	void setR(float val);
	void setG(float val);
	void setB(float val);
	void setOpacity(float val);
};



#endif
