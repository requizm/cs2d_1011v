#ifndef GEO_H
#define GEO_H


#include <cmath>

struct vec
{
	float x, y, z;

	vec() { x = y = z = 0; }
	vec(float a, float b, float c) : x(a), y(b), z(c) {}

	vec& sub(const vec& o);
	const float squaredlen();
	const float magnitude();
	const float dist(const vec& e, vec& t);

	//scale vec at a fixed point with equal dimension for screen drawing
	vec scaleFixedPoint(float scale, vec fixedPoint);
	//scale vec at a fixed point, for screen drawing, variable width & height
	vec scaleFixedPoint(float scalex, float scaley, vec fixedPoint);
};

struct vec2
{
	float x, y;
	//scale vec at a fixed point with equal dimension for screen drawing
	void scaleFixedPoint(float scale, vec2 fixedPoint);
};

#endif // !GEO_H