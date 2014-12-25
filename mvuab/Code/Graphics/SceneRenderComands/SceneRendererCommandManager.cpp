#include "SceneRenderComands\SceneRendererCommandManager.h"


CSceneRendererCommandManager::CSceneRendererCommandManager()
{
    
}

CSceneRendererCommandManager::~CSceneRendererCommandManager()
{
    
}

void CSceneRendererCommandManager::CleanUp()
{
	//m_SceneRendererCommands.Destroy();
}
std::string CSceneRendererCommandManager::GetNextName()
{
    return "";
}

bool CSceneRendererCommandManager::Load(const std::string &FileName)
{
    return true;
}

bool CSceneRendererCommandManager::Execute(CGraphicsManager &GM)
{
    return true;
}