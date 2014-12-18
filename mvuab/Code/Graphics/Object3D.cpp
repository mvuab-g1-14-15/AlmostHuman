#include "Object3D.h"
#include "Math\Matrix44.h"

CObject3D::CObject3D(const Math::Vect3f& pos, float32 yaw, float32 pitch, float32 roll, Math::Vect3f scale)
: m_Position(pos)
, m_fYaw(yaw)
, m_fPitch(pitch)
, m_fRoll(roll)
, m_Scale(scale)
{

}

CObject3D::CObject3D()
: m_Position( Math::Vect3f(0.f,0.f,0.f))
, m_fYaw(0.f)
, m_fPitch(0.f)
, m_fRoll(0.f)
, m_Scale(Math::Vect3f(1.0f,1.0f,1.0f))
{
}

Math::Mat44f CObject3D::GetTransform()
{
	Math::Mat44f l_MRotation, l_MTranslate, l_MScale;
	l_MRotation.SetIdentity();
	l_MTranslate.SetIdentity();
	l_MScale.SetIdentity();
	l_MRotation.SetFromAnglesYXZ(m_fYaw, m_fRoll, m_fPitch);
	l_MTranslate.Translate(m_Position);
	l_MScale.Scale(m_Scale.x, m_Scale.y, m_Scale.z);
	return l_MRotation * l_MTranslate * l_MScale;
}
