#include "Transform.h"


CTransform::CTransform( Math::Vect3f a_Position, Math::Vect3f a_Rotation, Math::Vect3f a_Scale )
	: CComponent( TRANSFORM )
	, m_Position( a_Position )
	, m_Rotation( a_Rotation )
	, m_Scale( a_Scale )
{
}


CTransform::~CTransform(void)
{
}
