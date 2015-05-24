#pragma once
#ifndef EFFECT_POOL_H
#define EFFECT_POOL_H

#include <d3dx9effect.h>

#include "XML/XMLTreeNode.h"

class CEffect;

class CEffectPool
{
    public:
        CEffectPool(void);
        virtual ~CEffectPool(void);
        bool Init();
        CEffect* CreateEffect( const CXMLTreeNode& aEffectNode );
    private:
        LPD3DXEFFECTPOOL mD3DXEffectPool;
};

#endif // EFFECT_POOL_H

