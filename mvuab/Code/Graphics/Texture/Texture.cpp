#include "Texture.h"
#include "Utils\Defines.h"
#include "GraphicsManager.h"

bool CTexture::LoadFile()
{
	HRESULT l_Res = D3DXCreateTextureFromFile(CGraphicsManager::GetSingletonPtr()->GetDevice(), m_FileName.c_str(), &m_Texture );
	return m_Texture != NULL;
}
void CTexture::Unload()
{
	CHECKED_RELEASE(m_Texture);
}

CTexture::CTexture() : m_Texture(), m_FileName("")
{

}

CTexture::~CTexture()
{
	Unload();
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