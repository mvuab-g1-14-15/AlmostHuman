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

class CLight : public CObject3D, public CName
{
public:
  enum TLightType
  {
    OMNI = 0,
    DIRECTIONAL,
    SPOT
  };
protected:
  //Members
  Math::CColor m_Color;
  TLightType m_Type;

  float m_Intensity;
  float m_StartRangeAttenuation;
  float m_EndRangeAttenuation;

  bool m_RenderShadows;
  bool m_GenerateDynamicShadowMap;
  bool m_GenerateStaticShadowMap;
  bool m_MustUpdateStaticShadowMap;

  CTexture* m_StaticShadowMap;
  CTexture* m_DynamicShadowMap;
  CTexture* m_ShadowMaskTexture;

  std::vector<CRenderableObjectsManager*>
  m_StaticShadowMapRenderableObjectsManagers;
  std::vector<CRenderableObjectsManager*>
  m_DynamicShadowMapRenderableObjectsManagers;

  Mat44f m_ViewShadowMap;
  Mat44f m_ProjectionShadowMap;

  //Method
  static TLightType GetLightTypeByName( const std::string& StrLightType );
public:
  CLight( ) {};
  CLight( const CXMLTreeNode& node );
  virtual ~CLight();
  void SetIntensity( const float intensity );
  float GetIntensity() const;
  void SetColor( const Math::CColor& Color );
  const Math::CColor& GetColor() const;
  void SetStartRangeAttenuation( const float StartRangeAttenuation );
  float GetStartRangeAttenuation() const;
  void SetEndRangeAttenuation( const float EndRangeAttenuation );
  float GetEndRangeAttenuation() const;
  bool RenderShadows() const;
  void SetType( const TLightType Type );
  TLightType GetType() const;
  virtual void Render() = 0;

  virtual void SetShadowMap( CGraphicsManager* GM ) = 0;
  //GET/SET
  void SetGenerateDynamicShadowMap( bool GenerateDynamicShadowMap );
  bool GetGenerateDynamicShadowMap() const;
  void SetGenerateStaticShadowMap( bool GenerateStaticShadowMap );
  bool GetGenerateStaticShadowMap() const;
  void SetMustUpdateStaticShadowMap( bool MustUpdateStaticShadowMap );
  bool GetMustUpdateStaticShadowMap() const;
  CTexture* GetStaticShadowMap() const;
  CTexture* GetDynamicShadowMap() const;
  CTexture* GetShadowMaskTexture() const;
  std::vector<CRenderableObjectsManager*>&
  GetStaticShadowMapRenderableObjectsManagers();
  std::vector<CRenderableObjectsManager*>&
  GetDynamicShadowMapRenderableObjectsManagers();
  void GenerateShadowMap( CGraphicsManager* GM );
  const Mat44f& GetViewShadowMap() const;
  const Mat44f& GetProjectionShadowMap() const;
  void BeginRenderEffectManagerShadowMap( CEffect* Effect );
};

#endif // LIGHT_H
