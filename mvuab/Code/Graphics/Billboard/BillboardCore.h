#ifndef _BillboardCore_H
#define _BillboardCore_H

#include "Utils\Name.h"
#include "Utils\Defines.h"
#include <vector>
#include "XML\XMLTreeNode.h"

class CBillboardInstance;
class CTexture;
class CEffectTechnique;
class CGraphicsManager;
class CRenderableVertexs;

class CBillboardCore : public CName
{
    public:
        CBillboardCore();
        virtual ~CBillboardCore();

        bool Init(const CXMLTreeNode& atts);
        void AddInstance( CBillboardInstance * aInstance );

        void Render( CRenderableVertexs* aRV, CGraphicsManager* aGM );
        void Update();
        void Flush();
    private:
        struct STextureStage
        {
            uint32 mStage;
            CTexture*           m_Texture;
        };
        
        typedef std::vector<STextureStage> TTextureStagedVector;
        TTextureStagedVector mTextures;

        CEffectTechnique*   mTechnique;
        float               mSize;
        float               mAlpha;
        bool                mUseTick;
        float               mTick;
        bool                mUseDeltaTime;
        bool                mFlipUVHorizontal;
        bool                mFlipUVVertical;
        typedef std::vector< CBillboardInstance* > TBillboardInstancesContainer;
        TBillboardInstancesContainer mInstances;
};

#endif //_BillboardCore_H
