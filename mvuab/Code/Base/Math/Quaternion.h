#pragma once

#include "Vector3.h"

class Quaternion
{
public:
	Quaternion() {};
	Quaternion(const Vect3f& n, float32 a);

public:
	const Quaternion Inverted() const;
	const Quaternion operator*(const Quaternion& q) const;
	const Vect3f operator*(const Vect3f& p) const;
	const Quaternion operator^(float32 t) const;
	const Quaternion Slerp(const Quaternion& r, float t) const;

	void ToAxisAngle(Vect3f& vecAxis, float32& flAngle) const;

public:
	float32 w;
	Vect3f v; // x, y, z
};