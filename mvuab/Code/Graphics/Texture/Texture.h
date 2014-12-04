#pragma once
#ifndef INC_TEXTURE_H_
#define INC_TEXTURE_H_

#include <d3dx9.h>
#include <string>

class CTexture
{
protected:
	LPDIRECT3DTEXTURE9 m_Texture;
	std::string m_FileName;
	virtual bool LoadFile();
	void Unload();

public:
	CTexture();
	virtual ~CTexture();

	const std::string & GetFileName() const;
	bool Load(const std::string &FileName);

	bool Reload();
	void Activate(size_t StageId);
};

#endif //INC_TEXTURE_H_