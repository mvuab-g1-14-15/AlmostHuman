#pragma once
#ifndef _SHAPES_H
#define _SHAPES_H

#include "Object3D.h"
#include "Math/Color.h"

class CRenderableVertexs;
class CEffectTechnique;

class CShape : public CObject3D
{
public:
	CShape();
	virtual ~CShape();
	virtual void Render( CEffectTechnique* aTechnique ) = 0;
protected:
	CEffectTechnique* mTechnique;
	Math::CColor	  mColor;
};

class CBoxShape : public CShape
{
public:
	CBoxShape();
	virtual ~CBoxShape();
	static void CreateGeometry();
	static void DestroyGeometry();
	virtual void Render( CEffectTechnique* aTechnique );
private:
	static CRenderableVertexs* sRV;
};

#endif // _SHAPES_H