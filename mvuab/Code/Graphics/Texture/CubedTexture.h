#pragma once
#ifndef INC_TEXTURE_H_
#define INC_TEXTURE_H_

#include "Texture.h"
class CCubedTexture
{
protected:
    virtual bool LoadFile();

public:
    CCubedTexture();
    virtual ~CCubedTexture();
};

#endif //INC_TEXTURE_H_