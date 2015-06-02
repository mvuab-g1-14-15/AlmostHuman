#pragma once
#ifndef INC_TEXTURE_MANAGER_H
#define INC_TEXTURE_MANAGER_H

#include <d3dx9.h>
#include <vector>
#include <map>
#include "Utils\Manager.h"
#include "Utils\MapManager.h"
#include "Texture.h"
#include "CubedTexture.h"
#include "GUITexture.h"

class CTextureManager : public CMapManager<CTexture>, public CManager
{
public:
  CTextureManager();
  CTextureManager( CXMLTreeNode& atts );
  ~CTextureManager();
  void Init();
  void Update() {}
  void Render() {}
  void Reload();
  CTexture* GetTexture( const std::string& fileName );
private:
	CTexture *m_DummyTexture;
};

#endif //INC_TEXTURE_MANAGER_H