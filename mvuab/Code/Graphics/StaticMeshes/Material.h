#pragma once
#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "Utils/Defines.h"
#include "XML/XMLTreeNode.h"
#include "Utils/Name.h"

class CTexture;

class CMaterial : public CName
{
public:
   CMaterial( const CXMLTreeNode& aNode );
   uint32 GetCount();
   void ApplyMaterial( uint32 aIdx );
   virtual ~CMaterial();
private:
  enum ETextureType
  {
    eColor = 0,
    eDiffuse,
    eNormal,
  };
  struct SSubMaterial
  {
    ETextureType mType;
    std::string  mTextureName;
    CTexture*    mTexture;
    Math::CColor mColor;
  };

  std::vector< SSubMaterial > mSubMaterials;


};

#endif //INC_STATIC_MESH_H_