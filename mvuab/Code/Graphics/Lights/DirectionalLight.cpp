#include "DirectionalLight.h"
#include "Math\Vector3.h"

void CDirectionalLight::SetDirection(const Math::Vect3f &Direction)
{
	m_Direction = Direction;
}

Math::Vect3f CDirectionalLight::GetDirection() const
{
	return m_Direction;
}

void CDirectionalLight::Render()
{
}