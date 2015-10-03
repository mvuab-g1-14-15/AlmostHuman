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

CMaterial::CMaterial( const std::string& FileName )
  : CName( FileName )
{
	std::FILE* l_pFile = 0;
	fopen_s(&l_pFile, FileName.c_str(), "rb");

	if (l_pFile)
	{
		unsigned short int l_header = 0;
		std::fread(&l_header, sizeof( unsigned short int ), 1, l_pFile);

		if (l_header == 0x55ff)
		{
			unsigned short int l_SubMeshes = 0;
			std::fread(&l_SubMeshes, sizeof(unsigned short int), 1, l_pFile);

			for (unsigned int i = 0; i < l_SubMeshes; ++i)
			{
				// Obtain all the textures if any
				unsigned short l_numTexturas = 0;
				std::fread( &l_numTexturas, sizeof( unsigned short int ), 1, l_pFile );
				SSubMaterial lSubMaterial;
				for (unsigned int j = 0; j < l_numTexturas; ++j)
				{
					unsigned short l_TextureLength = 0;
					std::fread(&l_TextureLength, sizeof(unsigned short int), 1, l_pFile);

					++l_TextureLength;
					char* textureName = (char *) malloc(sizeof(char) * (l_TextureLength));
					ZeroMemory(textureName, sizeof(char) * l_TextureLength);

					std::fread( textureName, sizeof( char ) * l_TextureLength, 1, l_pFile );
					CTexture* t = TextureMInstance->GetTexture( "Data/textures/" + std::string( textureName ) );
					if (t) { lSubMaterial.m_Textures.push_back(t); }
					free( textureName );
				}
				mSubMaterials.push_back(lSubMaterial);
			}
		}
		std::fclose(l_pFile);
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
	for( uint32 i = 0, lCount = mSubMaterials[aIdx].m_Textures.size(); i < lCount; ++i )
	{
		mSubMaterials[aIdx].m_Textures[i]->Activate(i);
	}
}