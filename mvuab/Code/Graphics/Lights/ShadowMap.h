#ifndef LENS_FLARE_H
#define LENS_FLARE_H

#include "XML/XMLTreeNode.h"
#include "Utils/Name.h"
#include <vector>

class CTexture;

class CShadowMap
{
    public:
        CShadowMap( const CXMLTreeNode& node );
        ~CShadowMap();

    protected:
        bool                    mGenerate;
        CTexture*               mTexture;
        Math::CColor            mColor;
        Math::Vect2u            mSize;
};

#endif // LENS_FLARE_H
