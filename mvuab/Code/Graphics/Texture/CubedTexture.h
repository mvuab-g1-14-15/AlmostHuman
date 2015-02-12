#pragma once
#ifndef INC_CUBED_TEXTURE_H_
#define INC_CUBED_TEXTURE_H_

#include "Texture.h"

class CCubedTexture : public CTexture
{
protected:
    virtual bool LoadFile();

	LPDIRECT3DCUBETEXTURE9 m_CubeTexture;

public:
    CCubedTexture();
    virtual ~CCubedTexture();

	virtual void Activate(size_t StageId);
};

#endif //INC_CUBED_TEXTURE_H_