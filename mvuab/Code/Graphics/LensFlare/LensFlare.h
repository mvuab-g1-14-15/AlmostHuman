#ifndef LENS_FLARE_H
#define LENS_FLARE_H

#include "XML/XMLTreeNode.h"
#include "Utils\Name.h"
#include "Utils/Manager.h"
#include <vector>
#include "Utils/TemplatedVectorMapManager.h"

class CTexture;
class CEffectTechnique;

class CLensFlare : public CName
{
    public:
        CLensFlare();
        ~CLensFlare();
        bool Init(const CXMLTreeNode& node );
        void Render(const Math::Vect2u& aLightProjPos, const Math::Vect2u&aCameraProjPos, float lAspectRatio );

    protected:
        struct SFlare
        {
            float        mDistance; // The proportional distance along the axis from the light to the center of the screen
            float        mSize;     // The normalize size of the element before scaling
            float        mMaxSize;  // The max size for the flare
            float        mAlpha;    // The alpha of the flare
            Math::Vect3f mColor;    // The color of the flare
            CTexture*    mTexture;  // The texture of the flare
        };

        std::vector<SFlare> mFlares;
        CEffectTechnique*    mTechnique;
};

#endif // LENS_FLARE_H
