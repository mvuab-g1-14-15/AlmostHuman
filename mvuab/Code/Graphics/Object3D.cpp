#include "Object3D.h"
#include "Math\Matrix44.h"

CObject3D::CObject3D(const Vect3f& pos, float32 yaw, float32 pitch, float32 roll)
: m_Position(pos)
, m_fYaw(yaw)
, m_fPitch(pitch)
, m_fRoll(roll)
{

}

CObject3D::CObject3D()
: m_Position( Vect3f(0.f,0.f,0.f))
, m_fYaw(0.f)
, m_fPitch(0.f)
, m_fRoll(0.f)
{
}

Mat44f CObject3D::GetTransform()
{
	Mat44f l_Identity;
	l_Identity.SetIdentity();
	return l_Identity;
}
