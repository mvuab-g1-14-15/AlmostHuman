#include "XML\XMLTreeNode.h"
//#include "Base.h"
#include "Logger\Logger.h"
#include "Core.h"
#include "PhysXObjManager.h"
#include "PhysicsManager.h"

////----PhysX Includes-------------
#undef min
#undef max
/*#include "NxPhysics.h"
#include "NxControllerManager.h"
#include "NxCapsuleController.h"
#include "NxActor.h"*/
#include "PhysicsManager.h"
#include "Actor/PhysicActor.h"
#include "Reports/PhysicTriggerReport.h"
#include "Cooking Mesh/PhysicCookingMesh.h"
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
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_Filename.c_str()))
	{
		std::string msg_error = "CPhysXObjManager::Load->Error al intentar leer el archivo xml: " + m_Filename;
		CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}

	CXMLTreeNode l_xml = newFile["physx_objs"];
	if( l_xml.Exists() )
	{
		uint16 l_Count = l_xml.GetNumChildren();

		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_Type = l_xml(i).GetName();

			if( l_Type == "physx_obj" )
			{
				std::string name = l_xml(i).GetPszProperty("name", "", true);
				int type = l_xml(i).GetIntProperty("type", 0, true);
				Math::Vect3f pos = l_xml(i).GetVect3fProperty("pos", Math::Vect3f(0, 0, 0), true);
				float yaw = l_xml(i).GetFloatProperty("yaw", 0.0f, true);
				float pitch = l_xml(i).GetFloatProperty("pitch", 0.0f, true);
				float roll = l_xml(i).GetFloatProperty("roll", 0.0f, true);
				std::string groupName = l_xml(i).GetPszProperty("group", "ECG_ESCENE", true);
				bool activeStart = l_xml(i).GetBoolProperty("active_startup", false, true);

				TPhysXObj* pxObj = NULL;
				
				if(type == PHYSX_OBJ_BOX)
				{
					TPhysXObjBox* pxBox = new TPhysXObjBox();

					pxBox->m_Dimensions = l_xml(i).GetVect3fProperty("dimension", Math::Vect3f(0, 0, 0), true);

					pxObj = pxBox;
				}

				assert(pxObj);

				pxObj->m_Type = type;
				pxObj->m_Group = CCore::GetSingletonPtr()->GetPhysicsManager()->GetCollisionGroup(groupName);
				pxObj->SetName(name);
				pxObj->SetPosition(pos);
				pxObj->SetYaw(yaw);
				pxObj->SetPitch(pitch);
				pxObj->SetRoll(roll);

				/*bool isOk = */this->AddResource(name, pxObj);
				//assert(isOk);

				if(activeStart)
				{
					CPhysicUserData* l_pPhysicUserDataMesh = new CPhysicUserData( pxObj->GetName()  );

					l_pPhysicUserDataMesh->SetGroup(static_cast<ECollisionGroup>(pxObj->m_Group));

					Math::Vect3f rotationVect = v3fZERO;

					rotationVect.x = Math::Utils::Deg2Rad(pxObj->GetPitch());
					rotationVect.y = Math::Utils::Deg2Rad(pxObj->GetYaw());
					rotationVect.z = Math::Utils::Deg2Rad(pxObj->GetRoll());

					TPhysXObjBox* pxBox = static_cast<TPhysXObjBox*>(pxObj);

					Math::Vect3f size = pxBox->m_Dimensions;
					size /= 2;
					
					CPhysicActor* l_MeshActor = new CPhysicActor(l_pPhysicUserDataMesh);
					l_pPhysicUserDataMesh->SetPaint (true);

					l_MeshActor->AddBoxShape(size, pxBox->GetPosition(), Math::Vect3f(0, 0, 0), rotationVect, NULL, (uint32)pxObj->m_Group);

					CCore::GetSingletonPtr()->GetPhysicsManager()->AddPhysicActor(l_MeshActor);
				}
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}