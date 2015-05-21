#ifndef _BILLBOARD_H
#define _BILLBOARD_H

#include "Utils\Name.h"
#include "Utils\Defines.h"
#include "Object3D.h"

#include "Math\Vector3.h"
#include "Math\Matrix44.h"

class CTexture;
class CRenderableVertexs;
class CEffectTechnique;

class CBillboard : public CName, public CObject3D
{
    public:
        CBillboard();
        virtual ~CBillboard();

		static void CreateBillBoardGeometry();
		static void DestroyBillBoardGeometry();

        bool Init(const CXMLTreeNode& atts);
        /*TODO: Alex Billboardbool Init
        (
            const std::string   &aName,
            const Math::Vect3f  &aPosition,
            const Math::Vect2f  &aSize,
            const std::string   &aTextureName,
            const std::string   &aTechniqueName,
            bool                 aActive = true
        );*/

        void Render();
        void Update();

        const CTexture*       GetTexture() const;
        const Math::Vect2f &  GetSize() const;

        void SetTexture(const CTexture *aTexture);
        void SetSize(const Math::Vect2f & aSize );
    private:
        CTexture*           m_Texture;
        CEffectTechnique*   mTechnique;
        Math::Vect2f        mSize;
        bool                m_Active;

		static CRenderableVertexs* sRV;
};

#endif //_BILLBOARD_H