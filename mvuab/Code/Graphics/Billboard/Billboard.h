#ifndef _BILLBOARD_H
#define _BILLBOARD_H

#include "Utils\Name.h"
#include "Utils\Defines.h"
#include "Math\Vector3.h"
#include "Utils\Manager.h"

//---Forward Declarations--
class CTexture;
class CRenderableVertexs;
//-------------------------


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

	    void SetSize(float sx, float sy);
        void SetPosition(Math::Vect3f l_Pos);

    private:
	    CTexture*			m_Texture;
        CRenderableVertexs* m_RV;

	    Math::Vect3f		m_Position;
	    Math::Vect3f		m_PosA;
	    Math::Vect3f		m_PosB;
	    Math::Vect3f		m_PosC;
	    Math::Vect3f		m_PosD;

        float				m_SizeX;
        float               m_SizeY;
	    bool				m_Active;
};

#endif //_BILLBOARD_H