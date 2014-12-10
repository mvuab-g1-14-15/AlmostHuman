#include "AnimatedModelsManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"

CAnimatedModelManager::CAnimatedModelManager()
{

}

CAnimatedModelManager::~CAnimatedModelManager()
{

}

CAnimatedCoreModel * CAnimatedModelManager::GetCore(const std::string &Name, const std::string &Path)
{
    return 0;
}

CAnimatedInstanceModel * CAnimatedModelManager::GetInstance(const std::string &Name)
{
    return 0;
}

void CAnimatedModelManager::Load(const std::string &Filename)
{
	m_FileName = Filename;
	
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_FileName.c_str()))
	{
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CStaticMeshManager::Load No se puede abrir \"%s\"!", m_FileName.c_str());
		return;
	}

	/*CXMLTreeNode node = newFile["animated_models"];
	if(!node.Exists())
	{
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CStaticMeshManager::Load Tag \"%s\" no existe",  "static_meshes");
		return;
	}

	for(int i = 0; i < node.GetNumChildren(); i++)
	{
		std::string name = node(i).GetPszProperty("name", "no_name");
		std::string file = node(i).GetPszProperty("filename", "no_file");

		CStaticMesh *l_StaticMesh = new CStaticMesh();
		if(!l_StaticMesh->Load(file))
		{
			delete l_StaticMesh;
		}
		else
		{
			AddResource(name, l_StaticMesh);
		}
	}
	
	*/
}