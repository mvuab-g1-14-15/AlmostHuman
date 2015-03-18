#ifndef _BILLBOARD_H
#define _BILLBOARD_H

#include "Utils\Name.h"
#include "Utils\Defines.h"
#include "Math\Vector3.h"
//---Forward Declarations--
class CTexture;
//-------------------------

class CRenderableVertexs;
class CBillboard : public CName
{
public:
	CBillboard();
	~CBillboard();
	void Init(Math::Vect3f Position, float Size, std::string Texture);
	void Update();
	void Render();

	void SetTexture(std::string Texture);
	CTexture* GetTexture();
private:
	CTexture*			m_Texture;
	float				m_Size;
	Math::Vect3f		m_Position;
	Math::Vect3f		m_PosA;
	Math::Vect3f		m_PosB;
	Math::Vect3f		m_PosC;
	Math::Vect3f		m_PosD;
	bool				m_Active;
	CRenderableVertexs* m_RV;
};

#endif //_BILLBOARD_H