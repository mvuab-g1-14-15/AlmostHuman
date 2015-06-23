#include "XML\XMLTreeNode.h"
//#include "Base.h"
#include "Logger\Logger.h"

#include "PhysXObjManager.h"
#include "PhysicsManager.h"
#include "EngineManagers.h"

////----PhysX Includes-------------
#undef min
#undef max
/*  #include "NxPhysics.h"
    #include "NxControllerManager.h"
    #include "NxCapsuleController.h"
    #include "NxActor.h"*/
#include "PhysicsManager.h"
#include "Actor/PhysicActor.h"
#include "Reports/PhysicTriggerReport.h"
#include "CookingMesh/PhysicCookingMesh.h"
#include "PhysicUserData.h"
////--------------------------------

#if defined(_DEBUG)
    #include "Memory\MemLeaks.h"
#endif

#include "Utils\Name.h"

CPhysXObjManager::CPhysXObjManager()
    : m_Filename("")
{

}

CPhysXObjManager::~CPhysXObjManager()
{
    CleanUp();
}

void CPhysXObjManager::CleanUp()
{
    this->Destroy();
}


bool CPhysXObjManager::Load( const std::string& filename )
{
    m_Filename = filename;

    return Reload();
}

bool CPhysXObjManager::Reload()
{
    // @TODO@ Ruly Revisar que este reload funcione
    bool lOk = true;
    CXMLTreeNode newFile, l_xml;

    if( newFile.LoadAndFindNode(m_Filename.c_str(), "physx_objs", l_xml ))
    {
        for(uint32 i = 0, lCount = l_xml.GetNumChildren(); i < lCount; ++i)
        {
            const CXMLTreeNode& lCurrentNode = l_xml(i);
            if( std::string( lCurrentNode.GetName() ) == "physx_obj" )
            {
                int32 type = lCurrentNode.GetAttribute<int32>("type", 0);

                ASSERT( type == PHYSX_OBJ_BOX, "Invalid type for phx object" );

                CPhysXObj *lpxObj = new CPhysXObj( lCurrentNode );

                if( AddResource(lpxObj->GetName(), lpxObj) )
                {
                    if(lCurrentNode.GetAttribute<bool>("active_startup", false ) )
                    {
                        CPhysicUserData* l_pPhysicUserDataMesh = new CPhysicUserData( lpxObj->GetName()  );
                        l_pPhysicUserDataMesh->SetGroup(static_cast<ECollisionGroup>(lpxObj->GetGroup()));

                        // @TODO@ Ruly Esto no tengo muy claro que funcione bien, porque las rotaciones de 3dObject ya estan en radianes, asi que puede las rotaciones del xml no esten muy bien.
                        Math::Vect3f rotationVect
                        (
                            Math::Utils::Deg2Rad(lpxObj->GetPitch()),
                            Math::Utils::Deg2Rad(lpxObj->GetYaw()),
                            Math::Utils::Deg2Rad(lpxObj->GetRoll())
                        );

                        CPhysicActor* l_MeshActor = new CPhysicActor(l_pPhysicUserDataMesh);
                        l_pPhysicUserDataMesh->SetPaint (true);

                        l_MeshActor->AddBoxShape
                        (
                            lpxObj->GetDimensions() / 2.0f,
                            lpxObj->GetPosition(),
                            Math::Vect3f(0, 0, 0),
                            rotationVect,
                            NULL,
                            (uint32)lpxObj->GetGroup()
                        );

                        PhysXMInstance->AddPhysicActor(l_MeshActor);
                    }
                }
            }
        }
    }
    else
    {
        lOk = false;
    }

    return lOk;
}