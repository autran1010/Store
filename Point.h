
#ifndef POINT_H_
#define	POINT_H_

class point {
private:
	float x, y;
public:
	point();
	point(float, float);
	~point();

	float getX() const;
	float getY() const;

	void setX(float);
	void setY(float);

};

#endif

