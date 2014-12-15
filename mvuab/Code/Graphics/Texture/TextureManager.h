#pragma once
#ifndef INC_TEXTURE_MANAGER_H
#define INC_TEXTURE_MANAGER_H

#include <d3dx9.h>
#include <vector>
#include <map>
#include "Utils\SingletonPattern.h"
#include "Utils\MapManager.h"
#include "Texture.h"

class CTextureManager : public CMapManager<CTexture>, public CSingleton<CTextureManager>
{
public:
    CTextureManager();
    ~CTextureManager();
    void Reload ();
    CTexture* GetTexture(const std::string fileName );
};

#endif //INC_TEXTURE_MANAGER_H