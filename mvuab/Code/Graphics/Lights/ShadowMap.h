#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include "XML/XMLTreeNode.h"
#include "Utils/Name.h"
#include "Texture/Texture.h"
#include <vector>

class CRenderableObjectsManager;

class CShadowMap
{
    public:
        CShadowMap( const CXMLTreeNode& node, const std::string& );
        ~CShadowMap();

        bool Generate();
        bool Activate();

    protected:
        bool                                    mGenerate;
        CTexture*                               mTexture;
        Math::CColor                            mColor;
        Math::Vect2i                            mSize;
        std::string                             mLayer;
        CTexture::TFormatType                   mFormatType;
        u_int                                   mClearMask;
        u_int                                   mStage;
        std::vector<CRenderableObjectsManager*> mROMs;
};

#endif // SHADOW_MAP_H
