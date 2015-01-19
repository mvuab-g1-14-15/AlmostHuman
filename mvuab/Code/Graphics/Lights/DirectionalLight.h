#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"
#include "XML\XMLTreeNode.h"

class CDirectionalLight : public CLight
{
protected:
	Math::Vect3f m_Direction;
public:
	CDirectionalLight(CXMLTreeNode node);
	void SetDirection(const Math::Vect3f &Direction);
	Math::Vect3f GetDirection() const;
	virtual void Render();
};

#endif // DIRECTIONAL_LIGHT_H
