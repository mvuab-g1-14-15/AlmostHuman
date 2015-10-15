#pragma once
#ifndef INC_MATERIAL_H_
#define INC_MATERIAL_H_

#include <vector>

class CTexture;
class CEffectTechnique;

class CMaterial
{
    public:
        CMaterial();
        virtual ~CMaterial();

        void Apply( CEffectTechnique* aTechnique );
        void AddTexture( CTexture* aTexture )           { mTextures.push_back( aTexture); }
        void SetSelfIlumAmount( float aSelfIlumAmount ) { mSelfIlumAmount = aSelfIlumAmount; }

    private:
        typedef std::vector<CTexture*> TTextureVector;
        TTextureVector mTextures;
        float mSelfIlumAmount;
};

#endif //INC_MATERIAL_H_