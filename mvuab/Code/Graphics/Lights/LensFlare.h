#ifndef LENS_FLARE_H
#define LENS_FLARE_H

#include "XML/XMLTreeNode.h"
#include <vector>

class CTexture;

class CLensFlare
{
    public:
        CLensFlare( const CXMLTreeNode& node );
        ~CLensFlare();

    protected:
        std::vector< CTexture*> mTextures;
        Math::CColor            mColor;
};

#endif // LENS_FLARE_H
