#ifndef _RENDERER_H
#define _RENDERER_H

#include "Component.h"

#include "Math/Vector3.h"
#include "Utils\Defines.h"

class CRenderableObject;

class CRenderer :
	public CComponent
{
public:
	CRenderer(void);
	virtual ~CRenderer(void);

  GET_SET_PTR(CRenderableObject, RenderableObject);

private:
  CRenderableObject* m_pRenderableObject;

};

#endif //_RENDERER_H