#include "TextureManager.h"

CTextureManager::CTextureManager()
{

}

CTextureManager::~CTextureManager()
{
	Destroy();
}

void CTextureManager::Reload ()
{
	std::map<std::string, CTexture*>::iterator itb = m_Resources.begin(), ite = m_Resources.end();
		
	for(; itb != ite; ++itb)
	{
		itb->second->Reload(); 
	}
}
