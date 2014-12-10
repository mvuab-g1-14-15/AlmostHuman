#include "AnimatedModelsManager.h"
#include "AnimatedCoreModel.h"
#include "AnimatedInstanceModel.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"

CAnimatedModelsManager::CAnimatedModelsManager()
{

}

CAnimatedModelsManager::~CAnimatedModelsManager()
{

}

CAnimatedCoreModel * CAnimatedModelsManager::GetCore(const std::string &Name, const std::string &Path)
{
    return 0;
}

CAnimatedInstanceModel * CAnimatedModelsManager::GetInstance(const std::string &Name)
{
    return 0;
}

void CAnimatedModelsManager::Load(const std::string &Filename)
{
	m_FileName = Filename;
	
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_FileName.c_str()))
	{
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CAnimatedModelManager::Load No se puede abrir \"%s\"!", m_FileName.c_str());
		return;
	}

	CXMLTreeNode node = newFile["animated_models"];
	if(!node.Exists())
	{
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CAnimatedModelManager::Load Tag \"%s\" no existe",  "animated_models");
		return;
	}

	for(uint32 i = 0; i < node.GetNumChildren(); ++i)
	{
		const std::string &name = node(i).GetPszProperty("name", "no_name");
		const std::string &path = node(i).GetPszProperty("path", "no_file");

		CAnimatedCoreModel *l_pAnimatedCoreModel = new CAnimatedCoreModel();
		if(!l_pAnimatedCoreModel->Load(path))
		{
			CHECKED_DELETE(l_pAnimatedCoreModel);
		}
		else
			AddResource(name, l_pAnimatedCoreModel);
	}
}