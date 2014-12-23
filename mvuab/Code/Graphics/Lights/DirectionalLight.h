#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class CGraphicsManager;

class CDirectionalLight : public CLight
{
protected:
	Math::Vect3f m_Direction;
public:
	CDirectionalLight() : CLight() {}
	void SetDirection(const Math::Vect3f &Direction);
	Math::Vect3f GetDirection() const;
	virtual void Render(CGraphicsManager *gm);
};

#endif // DIRECTIONAL_LIGHT_H
