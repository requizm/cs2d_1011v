#include "Geometry.h"

vec& vec::sub(const vec& o)
{
	x -= o.x; y -= o.y; z -= o.z; return *this;
}

const float vec::squaredlen()
{
	return x * x + y * y + z * z;
}
const float vec::magnitude()
{
	return sqrtf(squaredlen());
}

const float vec::dist(const vec& e, vec& t)
{
	t = *this; t.sub(e); return t.magnitude();
}

//scale vec at a fixed point with equal dimension for screen drawing
vec vec::scaleFixedPoint(float scale, vec fixedPoint)
{
	vec newVec;
	newVec.x = x * scale + fixedPoint.x * (1 - scale);
	newVec.y = y * scale + fixedPoint.y * (1 - scale);
	return newVec;
}

//scale vec at a fixed point, for screen drawing, variable
vec vec::scaleFixedPoint(float scalex, float scaley, vec fixedPoint)
{
	vec newVec;
	newVec.x = x * scalex + fixedPoint.x * (1 - scalex);
	newVec.y = y * scaley + fixedPoint.y * (1 - scaley);
	return newVec;
}

void vec2::scaleFixedPoint(float scale, vec2 fixedPoint)
{
	//scale = scale * fWindowScale;
	x = x * scale + fixedPoint.x * (1 - scale);
	y = y * scale + fixedPoint.y * (1 - scale);
}