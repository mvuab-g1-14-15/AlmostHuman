#pragma once
#ifndef EFFECT_POOL_H
#define EFFECT_POOL_H

#include "XML/XMLTreeNode.h"
#include "dx9.h"

class CEffect;
class CSharedEffect;

class CEffectPool
{
    public:
        CEffectPool(void);
        virtual ~CEffectPool(void);
        bool Init();
        LPD3DXEFFECTPOOL GetD3DEffectPool();
        CSharedEffect*   GetSharedEffect();
        void Bind();
    private:
        LPD3DXEFFECTPOOL mD3DXEffectPool;
        CSharedEffect*   mSharedEffect;
};

#endif // EFFECT_POOL_H

