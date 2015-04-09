#ifndef _BILLBOARD_H
#define _BILLBOARD_H

#include "Utils\Name.h"
#include "Utils\Defines.h"
#include "Math\Vector3.h"
#include "Utils\Manager.h"
//---Forward Declarations--
class CTexture;
//-------------------------

class CRenderableVertexs;
class CBillboard : public CName, public CManager
{
public:
	CBillboard();
	CBillboard(CXMLTreeNode& atts);
	~CBillboard();
	void Init();
	void Update();
	void Render();

	void SetTexture(std::string Texture);
	CTexture* GetTexture();
	GET_SET(float, Size);
	GET_SET(Math::Vect3f, Position);
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