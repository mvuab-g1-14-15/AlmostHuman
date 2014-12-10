
#include "GraphicsManager.h"
#include "AnimatedInstanceModel.h"
#include "Core.h"


CAnimatedInstanceModel::CAnimatedInstanceModel(const std::string &Name, const std::string &CoreName)
{
	SetName(Name);
	//m_StaticMesh = CCore::GetSingletonPtr()->GetStaticMeshManager()->
}
	
CAnimatedInstanceModel::~CAnimatedInstanceModel()
{
}

void CAnimatedInstanceModel::Render(CGraphicsManager *GM)
{
	//m_StaticMesh->Render(GM);
}