#ifndef LIGHT_H
#define LIGHT_H

#include "Object3D.h"
#include "Math/Color.h"
#include "Utils/Name.h"
#include "XML\XMLTreeNode.h"
#include "Math\Matrix44.h"

#include <string>
#include <vector>

#define STATIC_SHADOW_MAP_STAGE 5
#define DYNAMIC_SHADOW_MAP_STAGE 6
#define SHADOW_MAP_MASK_STAGE 7

using namespace Math;

class CTexture;
class CRenderableObjectsManager;
class CGraphicsManager;
class CEffect;
class CLensFlare;
class CShadowMap;
class CBillboardInstance;

class CLight : public CObject3D, public CName
{
    public:
        enum ELightType { OMNI = 0, DIRECTIONAL, SPOT };
    public:
        CLight();
        CLight( const CXMLTreeNode& node );
        virtual ~CLight();
        void                SetIntensity( const float intensity );
        float               GetIntensity() const;
        void                SetColor( const Math::CColor& Color );
        const Math::CColor& GetColor() const;
        void                SetStartRangeAttenuation( const float StartRangeAttenuation );
        float               GetStartRangeAttenuation() const;
        void                SetEndRangeAttenuation( const float EndRangeAttenuation );
        float               GetEndRangeAttenuation() const;
        void                SetType( const ELightType Type );
        ELightType          GetType() const;
        virtual void        Render() = 0;
        virtual void        SetShadowMap( CGraphicsManager *GM ) = 0;
        void                SetMustUpdateStaticShadowMap( bool MustUpdateStaticShadowMap );
        bool                GetMustUpdateStaticShadowMap() const;
        void                GenerateShadowMap( CGraphicsManager *GM );
        CShadowMap         *GetStaticShadowMap() const;
        CShadowMap         *GetDynamicShadowMap() const;
        CTexture           *GetShadowMaskTexture() const;
        const Mat44f&       GetViewShadowMap() const;
        const Mat44f&       GetProjectionShadowMap() const;
        void                BeginRenderEffectManagerShadowMap( CEffect *Effect );

        void SetRenderShadows( bool aBool )
        {
            m_RenderShadows = aBool;
        }

        CLensFlare* GetLensFlare()
        {
            return mLensFlare;
        }

        GET_SET(std::string, RoomName);

    protected:
        CLensFlare *mLensFlare;
        Math::CColor m_Color;
        ELightType m_Type;
        CBillboardInstance* mBillboard;

        float m_Intensity;
        float m_StartRangeAttenuation;
        float m_EndRangeAttenuation;

        // Shadow map data
        bool m_RenderShadows;
        bool m_MustUpdateStaticShadowMap;

        CTexture *m_ShadowMaskTexture;

        Mat44f m_ViewShadowMap;
        Mat44f m_ProjectionShadowMap;

        CShadowMap *mStaticShadowMap;
        CShadowMap *mDynamicShadowMap;

        std::string m_RoomName;

        void CreateGizmo();
};

#endif // LIGHT_H
