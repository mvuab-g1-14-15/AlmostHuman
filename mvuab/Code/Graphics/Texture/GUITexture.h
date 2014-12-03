#pragma once
#ifndef INC_TEXTURE_H_
#define INC_TEXTURE_H_

#include "Texture.h"
class CGUITexture
{
protected:
	virtual bool LoadFile();

public:
	CGUITexture();
	virtual ~CGUITexture();
};

#endif //INC_TEXTURE_H_