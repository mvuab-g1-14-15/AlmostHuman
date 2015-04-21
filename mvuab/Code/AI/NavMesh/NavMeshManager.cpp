#include "NavMeshManager.h"
#include "Utils\Defines.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "EngineManagers.h"
#include "NavMesh\NavMesh.h"

CNavMeshManager::CNavMeshManager(const CXMLTreeNode& atts)
	: CManager(atts)
{

}

CNavMeshManager::~CNavMeshManager()
{
}

void CNavMeshManager::Init()
{
	CXMLTreeNode newFile;
    if (!newFile.LoadFile(mConfigPath.c_str()))
    {
        LOG_ERROR_APPLICATION( "CStaticMeshManager::Load No se puede abrir \"%s\"!", mConfigPath.c_str());
        return;
    }

    CXMLTreeNode node = newFile["static_meshes"];
    if(!node.Exists())
    {
        LOG_ERROR_APPLICATION( "CStaticMeshManager::Load Tag \"%s\" no existe",  "static_meshes");
        return;
    }

	m_NavMesh = new CNavMesh();
	CStaticMeshManager* lStaticMeshManager = SMeshMInstance;
    for(int i = 0, lCount = node.GetNumChildren(); i < lCount ; ++i)
    {
        const std::string & lName = node(i).GetPszProperty("name", "no_name");
		const bool lIsNavigable = node(i).GetBoolProperty("navigable", false);
		
		if( lIsNavigable )
		{
			m_NavMesh->AddMesh( lStaticMeshManager->GetStaticMesh( lName ) );
		}
    }

	m_NavMesh->Calculate();
}

void CNavMeshManager::Update()
{
}

void CNavMeshManager::Render()
{
}