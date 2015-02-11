#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "Component.h"
#include "Math/Vector3.h"
#include "Utils\Defines.h"

class CTransform :
	public CComponent
{
public:
	CTransform( Math::Vect3f a_Position = Math::Vect3f(0.0f, 0.0f, 0.0f), Math::Vect3f a_Rotation = Math::Vect3f(0.0f, 0.0f, 0.0f), Math::Vect3f a_Scale = Math::Vect3f(1.0f, 1.0f, 1.0f) );
	virtual ~CTransform(void);

	GET_SET(Math::Vect3f, Position);
	GET_SET(Math::Vect3f, Rotation);
	GET_SET(Math::Vect3f, Scale);

private:
	Math::Vect3f m_Position;
	Math::Vect3f m_Rotation; //We need to change this into a Quaternion
	Math::Vect3f m_Scale;
};

#endif //_TRANSFORM_H