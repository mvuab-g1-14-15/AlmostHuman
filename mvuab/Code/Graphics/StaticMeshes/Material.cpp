#include "Material.h"

#include "GraphicsManager.h"

#include "Texture\Texture.h"
#include "Texture\TextureManager.h"

#include "Logger\Logger.h"
#include "StaticMesh.h"

#include "Math/AABB.h"
#include "Effects/EffectManager.h"


#include "EngineManagers.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"

#include <cstdio>

CMaterial::CMaterial( const CXMLTreeNode& aNode )
  : CName( aNode.GetAttribute<std::string>("name", "no_name" ) )
{
  for( uint32 i = 0, lCount = aNode.GetNumChildren(); i < lCount; ++i )
  {
    const CXMLTreeNode& lNode = aNode(i);
    const std::string& lName = lNode.GetName();
    if( lName == "material")
    {
      const std::string& lDiffusse = lNode.GetAttribute<std::string>("diffuse", "" );
      if( lDiffusse != "")
      {
        SSubMaterial lSubMaterial;
        lSubMaterial.mType = eDiffuse;
        lSubMaterial.mTextureName = lDiffusse;
        lSubMaterial.mTexture = TextureMInstance->GetTexture( "Data/textures/" + lDiffusse );
        lSubMaterial.mColor   = lNode.GetAttribute<Math::CColor>("color", Math::colWHITE );
        mSubMaterials.push_back(lSubMaterial);
      }

      const std::string& lNormal = lNode.GetAttribute<std::string>("normal", "" );
      if( lNormal != "")
      {
        SSubMaterial lSubMaterial;
        lSubMaterial.mType = eNormal;
        lSubMaterial.mTextureName = lNormal;
        lSubMaterial.mTexture = TextureMInstance->GetTexture( "Data/textures/" + lNormal );
        lSubMaterial.mColor   = lNode.GetAttribute<Math::CColor>("color", Math::colWHITE );
        mSubMaterials.push_back(lSubMaterial);
      }
    }
  }
}
CMaterial::~CMaterial()
{

}
uint32 CMaterial::GetCount()
{
  return mSubMaterials.size();
}

void CMaterial::ApplyMaterial( uint32 aIdx )
{
  SSubMaterial lSubMaterial = mSubMaterials[aIdx];

  if( lSubMaterial.mTexture )
  {
    uint32 lSamplerStage = 0;
    switch( lSubMaterial.mType )
    {
      case eDiffuse: lSamplerStage = 0; break;
      case eNormal: lSamplerStage  = 1; break; // Todo check this when it has self ilum
    }
    
    lSubMaterial.mTexture->Activate(lSamplerStage);
  }
}