#include "EffectPool.h"
#include "Effect.h"
#include "Utils/Defines.h"
#include "Utils/StringUtils.h"

#define D3D_EFFECT_POOL_ERROR_CASE( param ) case param: LOG_ERROR_APPLICATION( "%s creating the effect pool", ""#param"" ); break;

//-----------------------------------------------------------------------------------------------------------------------------------------
CEffectPool::CEffectPool(void)
    : mD3DXEffectPool( 0 )
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------
CEffectPool::~CEffectPool(void)
{
   CHECKED_RELEASE( mD3DXEffectPool );
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
          default: LOG_ERROR_APPLICATION( "Unknown error creating the effect pool");
       }
    }

    return lOk;
}

LPD3DXEFFECTPOOL CEffectPool::GetD3DEffectPool()
{
   return mD3DXEffectPool;
}