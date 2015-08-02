#include "EffectPool.h"
#include "Effect.h"
#include "SharedEffect.h"
#include "Utils/Defines.h"
#include "Utils/StringUtils.h"

#define D3D_EFFECT_POOL_ERROR_CASE( param ) case param: LOG_ERROR_APPLICATION( "%s creating the effect pool", ""#param"" ); break;

//-----------------------------------------------------------------------------------------------------------------------------------------
CEffectPool::CEffectPool(void)
    : mD3DXEffectPool( 0 )
    , mSharedEffect( new CSharedEffect() )
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------
CEffectPool::~CEffectPool(void)
{
    mD3DXEffectPool = 0;
    //CHECKED_RELEASE( mD3DXEffectPool );
}

void CEffectPool::Bind()
{
    mSharedEffect->Bind();
}

//-----------------------------------------------------------------------------------------------------------------------------------------
bool CEffectPool::Init()
{
    bool lOk( true );

    HRESULT lHR = D3DXCreateEffectPool(&mD3DXEffectPool);

    if( lHR != S_OK )
    {
        lOk = false;
        switch(lHR)
        {
            D3D_EFFECT_POOL_ERROR_CASE(D3DERR_INVALIDCALL)
            D3D_EFFECT_POOL_ERROR_CASE(E_FAIL)
        default:
            LOG_ERROR_APPLICATION( "Unknown error creating the effect pool");
        }
    }

    return lOk;
}

LPD3DXEFFECTPOOL CEffectPool::GetD3DEffectPool()
{
    return mD3DXEffectPool;
}

CSharedEffect* CEffectPool::GetSharedEffect()
{
    return mSharedEffect;
}