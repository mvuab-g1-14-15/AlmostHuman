#pragma once
#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "Utils/Defines.h"
#include "Utils/Name.h"

class CTexture;

typedef std::vector<CTexture*> TTextureVector;
class CMaterial : public CName
{
public:
   CMaterial( const std::string& FileName );
   uint32 GetCount();
   void ApplyMaterial( uint32 aIdx );
   virtual ~CMaterial();
private:
  struct SSubMaterial
  {
    TTextureVector m_Textures;
  };

  std::vector< SSubMaterial > mSubMaterials;
};

#endif //INC_STATIC_MESH_H_