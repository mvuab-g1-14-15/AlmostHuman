#pragma once
#ifndef INC_TEXTURE_MANAGER_H
#define INC_TEXTURE_MANAGER_H

#include <vector>
#include <map>
#include "Utils\Manager.h"
#include "Utils\MapManager.h"
#include "Texture.h"
#include "CubedTexture.h"
#include "GUITexture.h"

static const char* sDummyTextureName = "../CommonData/textures/UV_alignment_pattern.jpg";

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
        CTexture* AddTexture( const std::string& fileName );
};

#endif //INC_TEXTURE_MANAGER_H