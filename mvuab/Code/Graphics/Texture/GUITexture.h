#pragma once
#ifndef INC_GUI_TEXTURE_H_
#define INC_GUI_TEXTURE_H_

#include "Texture.h"
class CGUITexture: public CTexture
{
protected:
    virtual bool LoadFile();

public:
    CGUITexture();
    virtual ~CGUITexture();
};

#endif //INC_GUI_TEXTURE_H_