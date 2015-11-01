#include "Cinematics\CinematicsElement\LoadASEElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Timer\Timer.h"
#include "PhysicsManager.h"
#include "CookingMesh\PhysicCookingMesh.h"
#include "Utils\PhysicUserData.h"

CLoadASEElement::CLoadASEElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
{
	m_bLoadLater = atts.GetAttribute<bool>("load_later", false);
	if(m_bLoadLater)
		m_Name = atts.GetAttribute<std::string>("resource_id_file", "");
	else
	{
		CXMLTreeNode l_Root, l_Node;
		if ( l_Root.LoadAndFindNode( atts.GetAttribute<std::string>("resource_id_file", "").c_str(), "ASE", l_Node ) )
		{
			CPhysicsManager *lPM = PhysXMInstance;
			for ( int i = 0, l_NumChilds = l_Node.GetNumChildren(); i < l_NumChilds; ++i )
			{
				CXMLTreeNode& l_CurrentNode = l_Node( i );
				const std::string lName = l_CurrentNode.GetAttribute<std::string>("name", "" );
				const std::string lFileName = l_CurrentNode.GetAttribute<std::string>("resource_id_file", "" );
				if (lPM->GetLoadASE())
				{
					if (lPM->GetCookingMesh()->CreateMeshFromASE(lFileName, lName, lName))
					{
						CPhysicUserData* l_pPhysicUserDataASEMesh = new CPhysicUserData( lName + "Escenario" );
						l_pPhysicUserDataASEMesh->SetColor( Math::colBLACK );
						CPhysicActor* l_AseMeshActor = new CPhysicActor( l_pPhysicUserDataASEMesh );

						VecMeshes l_CookMeshes = PhysXMInstance->GetCookingMesh()->GetMeshes(lName);

						for ( VecMeshes::iterator it = l_CookMeshes.begin(); it != l_CookMeshes.end(); it++ )
							l_AseMeshActor->AddMeshShape( it->second, Vect3f( 0, 0, 0 ) );
					
						l_AseMeshActor->SetKinematic(true);
						lPM->AddPhysicActor( l_AseMeshActor );
					}
				}
			}
		}
	}
}

void CLoadASEElement::Update()
{
	if(m_bLoadLater)
	{
		CXMLTreeNode l_Root, l_Node;
		if ( l_Root.LoadAndFindNode( m_Name.c_str(), "ASE", l_Node ) )
		{
			CPhysicsManager *lPM = PhysXMInstance;
			for ( int i = 0, l_NumChilds = l_Node.GetNumChildren(); i < l_NumChilds; ++i )
			{
				CXMLTreeNode& l_CurrentNode = l_Node( i );
				const std::string lName = l_CurrentNode.GetAttribute<std::string>("name", "" );
				const std::string lFileName = l_CurrentNode.GetAttribute<std::string>("resource_id_file", "" );
				if (lPM->GetLoadASE())
				{
					if (lPM->GetCookingMesh()->CreateMeshFromASE(lFileName, lName, lName))
					{
						CPhysicUserData* l_pPhysicUserDataASEMesh = new CPhysicUserData( lName + "Escenario" );
						l_pPhysicUserDataASEMesh->SetColor( Math::colBLACK );
						CPhysicActor* l_AseMeshActor = new CPhysicActor( l_pPhysicUserDataASEMesh );

						VecMeshes l_CookMeshes = PhysXMInstance->GetCookingMesh()->GetMeshes(lName);

						for ( VecMeshes::iterator it = l_CookMeshes.begin(); it != l_CookMeshes.end(); it++ )
							l_AseMeshActor->AddMeshShape( it->second, Vect3f( 0, 0, 0 ) );
					
						l_AseMeshActor->SetKinematic(true);
						lPM->AddPhysicActor( l_AseMeshActor );
					}
				}
			}
		}
	}
	m_CurrentTime += deltaTimeMacro;
}

void CLoadASEElement::Execute( CGraphicsManager& GM )
{

}

void CLoadASEElement::Render()
{
}

void CLoadASEElement::Render2D(CGraphicsManager &GM)
{
}