#include "Material.h"
#include "Texture/Texture.h"
#include "Effects/Effect.h"
#include "Effects/EffectTechnique.h"

CMaterial::CMaterial()
    : mSelfIlumAmount(0)
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::Apply( CEffectTechnique* aTechnique )
{
    // Set the textures
    for( uint32 i = 0, lCount = mTextures.size(); i < lCount; ++i )
    {
        mTextures[i]->Activate(i);
    }

    // Set the self ilum amount
    CEffect *lEffect = aTechnique->GetEffect();
    if( lEffect )
    {
        lEffect->SetSelfIlumAmount(mSelfIlumAmount);
    }
}