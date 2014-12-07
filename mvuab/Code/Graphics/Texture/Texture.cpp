#include "Texture.h"
#include "Utils\Defines.h"
#include "GraphicsManager.h"

CTexture::CTexture() : m_Texture(0), m_FileName("")
{

}

CTexture::~CTexture()
{
	Unload();
}

bool CTexture::LoadFile()
{
	HRESULT l_Res = D3DXCreateTextureFromFile(CGraphicsManager::GetSingletonPtr()->GetDevice(), ("./Data/textures/" + m_FileName).c_str(), &m_Texture );
    return(l_Res == S_OK);
}

void CTexture::Unload()
{
	CHECKED_RELEASE(m_Texture);
}

const std::string & CTexture::GetFileName() const
{
	return m_FileName;
}

bool CTexture::Load(const std::string &FileName)
{
	m_FileName = FileName;
	return LoadFile();
}
bool CTexture::Reload()
{
	Unload();
	return LoadFile();
}

void CTexture::Activate(size_t StageId)
{
	CGraphicsManager::GetSingletonPtr()->GetDevice()->SetTexture(StageId, m_Texture);
}