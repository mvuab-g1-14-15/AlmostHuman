#include "StaticMeshManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"

CStaticMeshManager::CStaticMeshManager()
{
	m_FileName = "";
}

CStaticMeshManager::~CStaticMeshManager()
{
	CMapManager::Destroy();
}

bool CStaticMeshManager::Load(const std::string &FileName)
{
	m_FileName = FileName;
	
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_FileName.c_str()))
	{
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CStaticMeshManager::Load No se puede abrir \"%s\"!", m_FileName.c_str());
		return false;
	}

	CXMLTreeNode node = newFile["static_meshes"];
	if(!node.Exists())
	{
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CStaticMeshManager::Load Tag \"%s\" no existe",  "static_meshes");
		return false;
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
	
	return true;
}

bool CStaticMeshManager::Reload()
{
	CMapManager::Destroy();
	return Load(m_FileName);
}