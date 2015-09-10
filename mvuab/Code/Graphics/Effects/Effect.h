#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include "Defines.h"
#include <string>
#include <vector>
#include "Math\Matrix44.h"
#include "Effects\Defines.h"
#include "Utils\Name.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"

class CLight;
class CEffectPool;

class CEffect: public CName
{
    public:
        CEffect( const std::string& EffectName );
        virtual ~CEffect();

        void LinkSemantics();
        bool SetLights( size_t NumOfLights );
        bool SetLight( size_t i_light );

        bool SetViewToLightMatrix( const Math::Mat44f& Matrix );

        bool Load( CXMLTreeNode& EffectNode, CEffectPool* aEffectPool );
        bool Reload(CEffectPool* aEffectPool);

        bool SetWorldMatrix( const Math::Mat44f& Matrix );
        bool SetInverseWorldMatrix( const Math::Mat44f& Matrix );
        bool SetWorldViewMatrix( const Math::Mat44f& Matrix );
        bool SetWorldViewProjectionMatrix( const Math::Mat44f& Matrix );

        //DirectX Methods Interface
        D3DXHANDLE GetTechniqueByName( const std::string& TechniqueName );

        GET_SET( D3DXHANDLE, ViewToLightProjectionMatrixParameter );

        GET_SET( D3DXHANDLE, LightEnabledParameter );
        GET_SET( D3DXHANDLE, LightsTypeParameter );
        GET_SET( D3DXHANDLE, LightsPositionParameter );
        GET_SET( D3DXHANDLE, LightsDirectionParameter );
        GET_SET( D3DXHANDLE, LightsAngleParameter );
        GET_SET( D3DXHANDLE, LightsColorParameter );
        GET_SET( D3DXHANDLE, LightsFallOffParameter );
        GET_SET( D3DXHANDLE, LightsStartRangeAttenuationParameter );
        GET_SET( D3DXHANDLE, LightsEndRangeAttenuationParameter );

        GET_SET( D3DXHANDLE, BonesParameter );

		GET_SET( D3DXHANDLE, LightProbesParameter );

        GET_SET( D3DXHANDLE, DebugColor );
        GET_SET( D3DXHANDLE, UseDebugColor );

        // Texture
        GET_SET( D3DXHANDLE, HeightTexture );
        GET_SET( D3DXHANDLE, WidthTexture );

        GET_SET_REF( std::string, FileName );
        GET_SET( LPD3DXEFFECT, Effect );

        const BOOL* GetLightsEnabled()
        {
            return m_LightsEnabled;
        }
        const int32* GetLightsType()
        {
            return m_LightsType;
        }
        const float32* GetLightsAngle()
        {
            return m_LightsAngle;
        }
        const float32* GetLightsFallOff()
        {
            return m_LightsFallOff;
        }
        const float32* GetLightsStartRangeAttenuation()
        {
            return m_LightsStartRangeAttenuation;
        }
        const float32* GetLightsEndRangeAttenuation()
        {
            return m_LightsEndRangeAttenuation;
        }
        const Math::Vect3f* GetLightsPosition()
        {
            return m_LightsPosition;
        }
        const Math::Vect3f* GetLightsDirection()
        {
            return m_LightsDirection;
        }
        const Math::Vect3f* GetLightsColor()
        {
            return m_LightsColor;
        }

        void SetShadowMapParameters( bool UseShadowMaskTexture, bool UseStaticShadowmap, bool UseDynamicShadowmap );
        void SetFog( bool aUseFog, float32 aFogStart, float32 aFogEnd, float32 aFogExponent, EFogFunction aFogFun );
        void SetDebugColor( bool aUse, const Math::CColor aColor );
        void SetSize( float aSize );
        void SetAlpha( float aAlpha );
        void SetAngle( float aAngle );
		void SetDirection( const Math::Vect3f& aDirection );

    protected:
        LPD3DXEFFECT m_Effect;
        virtual void  SetNullParameters();
        void GetParameterBySemantic( const char* SemanticName, D3DXHANDLE& a_Handle );
        void GetParameterBySemantic( const std::string& SemanticName, D3DXHANDLE& a_Handle );

    private: // Members
        std::string m_FileName;
        BOOL m_LightsEnabled[MAX_LIGHTS_BY_SHADER];
        int32 m_LightsType[MAX_LIGHTS_BY_SHADER];
        float32 m_LightsAngle[MAX_LIGHTS_BY_SHADER];
        float32 m_LightsFallOff[MAX_LIGHTS_BY_SHADER];
        float32 m_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER];
        float32 m_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER];
        Math::Vect3f m_LightsPosition[MAX_LIGHTS_BY_SHADER];
        Math::Vect3f m_LightsDirection[MAX_LIGHTS_BY_SHADER];
        Math::Vect3f m_LightsColor[MAX_LIGHTS_BY_SHADER];

		DECLARE_EFFECT_PARAMETER( Direction  );
        DECLARE_EFFECT_PARAMETER( Size  );
        DECLARE_EFFECT_PARAMETER( Angle );
        DECLARE_EFFECT_PARAMETER( Alpha );
        DECLARE_EFFECT_PARAMETER( Color );

        DECLARE_EFFECT_PARAMETER( InverseWorldMatrix );
        DECLARE_EFFECT_PARAMETER( WorldMatrix );
        DECLARE_EFFECT_PARAMETER( WVMatrix );
        DECLARE_EFFECT_PARAMETER( WVPMatrix );

        D3DXHANDLE m_ViewToLightProjectionMatrixParameter;

        D3DXHANDLE m_LightEnabledParameter, m_LightsTypeParameter,
                   m_LightsPositionParameter, m_LightsDirectionParameter, m_LightsAngleParameter,
                   m_LightsColorParameter;

        D3DXHANDLE m_LightsFallOffParameter, m_LightsStartRangeAttenuationParameter,
                   m_LightsEndRangeAttenuationParameter;

        D3DXHANDLE m_BonesParameter;

		D3DXHANDLE m_LightProbesParameter;

        D3DXHANDLE m_DebugColor;
        D3DXHANDLE m_UseDebugColor;

        //
        // Shadow map handles
        //
        D3DXHANDLE m_ShadowMapTextureSizeParameter;
        D3DXHANDLE m_UseShadowMaskTextureParameter;
        D3DXHANDLE m_UseStaticShadowmapParameter;
        D3DXHANDLE m_UseDynamicShadowmapParameter;

        D3DXHANDLE m_SceneTextureParameter;
        D3DXHANDLE m_BloomThresholdParameter;
        D3DXHANDLE m_SampleOffsetsParameter;
        D3DXHANDLE m_SampleWeightsParameter;
        D3DXHANDLE m_GaussianBlurTextureParameter;
        D3DXHANDLE m_BloomIntensityParameter;
        D3DXHANDLE m_BaseIntensityParameter;
        D3DXHANDLE m_BloomSaturationParameter;
        D3DXHANDLE m_BaseSaturationParameter;
        D3DXHANDLE m_PostBloomTextureParameter;

        // Fog information
        D3DXHANDLE m_UseFog;
        D3DXHANDLE m_FogStart;
        D3DXHANDLE m_FogEnd;
        D3DXHANDLE m_FogExp;
        D3DXHANDLE m_FogFun;

        //Width & Height Textures
        D3DXHANDLE m_HeightTexture;
        D3DXHANDLE m_WidthTexture;

        //Width & Height window
        D3DXHANDLE m_HeightWindow;
        //D3DXHANDLE m_WidthWindow;
        DECLARE_EFFECT_PARAMETER( WidthWindow )

        // To avoid memory leaks
        std::vector<char*> m_NamesMacrosChar;
        std::vector<char*> m_DescriptionsMacrosChar;

        // The macros to compile the effect
        std::vector<D3DXMACRO> m_Defines;

    private: // Methods

        bool LoadEffect( CEffectPool* aEffectPool );
        void Unload();
        void ResetLightsHandle();
};
#endif // EFFECT_H