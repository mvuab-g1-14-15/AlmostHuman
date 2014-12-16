#include "quaternion.h"

#include <cmath>

// Building a quaternion from an axis-angle rotation.
// http://youtu.be/SCbpxiCN0U0
Quaternion::Quaternion(const Vect3f& n, float32 a)
{
	// a will be our theta.
	// We must convert degrees to radians. (This step wasn't shown in the video.)
	// 360 degrees == 2pi radians
	a = a /360 * (float32)ePIf * 2;

	w = cos(a/2);

	v = n * sin(a/2);
}

// http://youtu.be/A6A0rpV9ElA
const Quaternion Quaternion::Inverted() const
{
	Quaternion q;
	q.w = w;
	q.v = -v;
	return q;
}

// Multiplying two quaternions together combines the rotations.
// http://youtu.be/CRiR2eY5R_s
const Quaternion Quaternion::operator*(const Quaternion& q) const
{
	Quaternion r;

	r.w = w*q.w - v.DotProduct(q.v);
	r.v = v*q.w + q.v*w + v.CrossProduct(q.v);

	return r;
}

// Rotate a vector with this quaternion.
// http://youtu.be/Ne3RNhEVSIE
// The basic equation is qpq* (the * means inverse) but we use a simplified version of that equation.
const Vect3f Quaternion::operator*(const Vect3f& V) const
{
	Quaternion p;
	p.w = 0;
	p.v = V;

	// Could do it this way:
	/*
	const Quaternion& q = (*this);
	return (q * p * q.Inverted()).v;
	*/

	// But let's optimize it a bit instead.
	Vect3f vcV = v.CrossProduct(V);
	return V + vcV*(2*w) + v.CrossProduct(vcV)*2;
}

// Raising this quaternion to a power of t gives us the
// fraction t of the rotation of the quaternion (phew!)
// http://youtu.be/x1aCcyD0hqE
const Quaternion Quaternion::operator^(float32 t) const
{
	// Convert the quaternion back into axis/angle
	float32 a;
	Vect3f n;

	ToAxisAngle(n, a);

	// Scale the angle by t
	float32 at = a*t;

	// Make a new quaternion out of the new value
	return Quaternion(n, at);
}

// Spherical linear interpolation of a quaternion
const Quaternion Quaternion::Slerp(const Quaternion& other, float32 t) const
{
	const Quaternion& q = *this;
	Quaternion r = other;

	// This is the quaternion equation that I discuss in the video, and it works just fine!
	// return ((r * q.Inverted()) ^ t) * q;

	// But this one is faster. For the derivation, see "3D Primer For Graphics
	// and Game Development" by Dunn and Parberry, section 10.4.13. I may go
	// over this one in a future video, but the two formulas come out the same.
	// (Hint: I'm more likely to do it if I get a request!)
	float32 flCosOmega = w*r.w + r.v.DotProduct(v);
	if (flCosOmega < 0)
	{
		// Avoid going the long way around.
		r.w = -r.w;
		r.v = -r.v;
		flCosOmega = -flCosOmega;
	}

	float32 k0, k1;
	if (flCosOmega > 0.9999f)
	{
		// Very close, use a linear interpolation.
		k0 = 1-t;
		k1 = t;
	}
	else
	{
		// Trig identity, sin^2 + cos^2 = 1
		float32 flSinOmega = sqrt(1 - flCosOmega*flCosOmega);

		// Compute the angle omega
		float32 flOmega = atan2(flSinOmega, flCosOmega);
		float32 flOneOverSinOmega = 1/flSinOmega;

		k0 = sin((1-t)*flOmega) * flOneOverSinOmega;
		k1 = sin(t*flOmega) * flOneOverSinOmega;
	}

	// Interpolate
	Quaternion result;
	result.w = q.w * k0 + r.w * k1;
	result.v = q.v * k0 + r.v * k1;

	return result;
}

void Quaternion::ToAxisAngle(Vect3f& vecAxis, float32& flAngle) const
{
	// Convert the quaternion back into axis/angle
    if (v.SquaredLength() < 0.0001f)
		vecAxis = Vect3f(1, 0, 0);
	else
        vecAxis = v.GetNormalized();

	//TAssert(fabs(vecAxis.LengthSqr() - 1) < 0.000001f);

	// This is the opposite procedure as explained in
	// http://youtu.be/SCbpxiCN0U0 w = cos(a/2) and a = acos(w)*2
	flAngle = acos(w)*2;

	// Convert to degrees
    flAngle *= 360 / ((float32)ePIf * 2);
}