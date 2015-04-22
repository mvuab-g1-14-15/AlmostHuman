#include "NavMeshManager.h"
#include "Utils\Defines.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "EngineManagers.h"
#include "NavMesh\NavMesh.h"

#include "GraphicsManager.h"

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

    CXMLTreeNode node = newFile["RenderableObjects"];
    if(!node.Exists())
    {
        LOG_ERROR_APPLICATION( "CStaticMeshManager::Load Tag \"%s\" no existe",  "static_meshes");
        return;
    }

	m_NavMesh = new CNavMesh();
	CStaticMeshManager* lStaticMeshManager = SMeshMInstance;
    for(int i = 0, lCount = node.GetNumChildren(); i < lCount ; ++i)
    {
		const std::string& TagName = node( i ).GetName();
		if ( TagName == "MeshInstance" )
		{
			const std::string & lName = node(i).GetPszProperty("core", "no_name");
			const bool lIsNavigable = node(i).GetBoolProperty("navigable", false);
			const Math::Vect3f lPos = node(i).GetVect3fProperty("pos", Math::Vect3f( 0.0f ));
		
			if( lIsNavigable )
			{
				m_NavMesh->AddMesh( lPos, lStaticMeshManager->GetStaticMesh( lName ) );
			}
		}
    }

	m_NavMesh->Calculate();
}

void CNavMeshManager::Update()
{
}

void CNavMeshManager::Render()
{
	Math::Mat44f t;
	t.Translate(m_NavMesh->m_Pos);
	CEngineManagers::GetSingletonPtr()->GetGraphicsManager()->DrawBox(t, m_NavMesh->m_sizeX, m_NavMesh->m_sizeY, m_NavMesh->m_sizeZ);
}