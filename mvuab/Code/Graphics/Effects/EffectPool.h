#pragma once
#ifndef EFFECT_POOL_H
#define EFFECT_POOL_H

#include "XML/XMLTreeNode.h"
#include "dx9.h"

class CEffect;

class CEffectPool
{
    public:
        CEffectPool(void);
        virtual ~CEffectPool(void);
        bool Init();
        LPD3DXEFFECTPOOL GetD3DEffectPool();
    private:
        LPD3DXEFFECTPOOL mD3DXEffectPool;
};

#endif // EFFECT_POOL_H

