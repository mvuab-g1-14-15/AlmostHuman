#pragma once
#ifndef RENDERABLE_OBJECT_H
#define RENDERABLE_OBJECT_H

#include "Object3D.h"
#include "Utils\Name.h"
#include "GraphicsManager.h"

class CRenderableObject : public CObject3D, public CName
{
public:
    CRenderableObject();
    virtual ~CRenderableObject();

    virtual void Update() {}
    virtual void Render() = 0;
};

#endif