#include "TestProcess.h"
#include "Utils\Defines.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "ActionManager.h"
#include "Fonts\FontManager.h"
#include "Object3D.h"
#include "Math\Matrix44.h"
#include "Logger\Logger.h"
#include "Utils\LogRender.h"
#include "Utils\Defines.h"
#include "Math\AABB.h"
#include "Core.h"
#include "Utils\BaseUtils.h"
#include "Timer\Timer.h"
#include "Periphericals\Mouse.h"

#include "ScriptManager\ScriptManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "StaticMeshes\InstanceMesh.h"
#include "StaticMeshes\StaticMesh.h"

#include "RenderableVertex\RenderableVertex.h"
#include "RenderableVertex\IndexedVertexs.h"
#include "RenderableVertex\VertexTypes.h"
#include "Texture\Texture.h"

#include "StaticMeshes\StaticMesh.h"
#include "Cameras\CameraFPShooter.h"
#include "Cameras\CameraManager.h"

#include "Effects\EffectManager.h"
#include "RenderableVertex\VertexTypes.h"

#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Utils/PhysicUserData.h"

#include <d3dx9.h>

#include "Items\Grenade.h"



CGrenade* p_Grenade;

void GetFilesFromPath(const std::string &Path, std::vector<std::string> &_OutFiles);

CTestProcess::CTestProcess() : CProcess(),
  m_Speed( 0.1f ),
  m_Amount( 0.0f ), m_Angle( 0.0f ),  m_AngleMoon( 0.0f ), m_PaintAll(false)
{
  CCameraManager::GetSingletonPtr()->NewCamera(CCamera::FirstPerson, "TestProcessCam", Math::Vect3f(-15.0f,2.0f,0.0f),
      Math::Vect3f(0.0f,2.0f,0.0f) );
  CCameraManager::GetSingletonPtr()->SetCurrentCamera("TestProcessCam");

  unsigned short debug = VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TANGENT|VERTEX_TYPE_BINORMAL|VERTEX_TYPE_TEXTURE1|VERTEX_TYPE_DIFFUSE;

}
bool done = false;

CTestProcess::~CTestProcess()
{
  CLogger::GetSingletonPtr()->SaveLogsInFile();
  CHECKED_DELETE(p_Grenade);
}

void CTestProcess::Update()
{
  m_Amount +=  0.01f;
  m_Angle  += deltaTime * 0.05f;
  m_AngleMoon += deltaTime * 0.05f;

  CActionManager* pActionManager = CActionManager::GetSingletonPtr();

  if( pActionManager->DoAction("ReloadStaticMesh") )
    CCore::GetSingletonPtr()->GetStaticMeshManager()->Reload();

  if( pActionManager->DoAction("ReloadLUA") )
    CCore::GetSingletonPtr()->GetScriptManager()->Reload();

  if( pActionManager->DoAction("ReloadShaders") )
    CCore::GetSingletonPtr()->GetEffectManager()->Reload();

  if( pActionManager->DoAction("ReloadActionToInput") )
    CCore::GetSingletonPtr()->GetActionManager()->Reload();

  if( pActionManager->DoAction("SaveActionToInput") )
    CCore::GetSingletonPtr()->GetActionManager()->SaveXML("Data/Prueba.xml");

  if( pActionManager->DoAction("SetActionToInput") )
  {
    std::vector<S_INPUT_ACTION> vecInputAction;
    S_INPUT_ACTION sInputAction;
    sInputAction.m_DeviceType = IDV_KEYBOARD;
    sInputAction.m_EventType = EVENT_DOWN_UP;
    sInputAction.m_Code = KEY_H;
    sInputAction.m_sDeviceType = "IDV_KEYBOARD";
    sInputAction.m_sEventType = "EVENT_DOWN_UP";
    sInputAction.m_sCode = "KEY_H";
    vecInputAction.push_back(sInputAction);
    sInputAction.m_DeviceType = IDV_KEYBOARD;
    sInputAction.m_EventType = EVENT_DOWN;
    sInputAction.m_Code = KEY_LCTRL;
    sInputAction.m_sDeviceType = "IDV_KEYBOARD";
    sInputAction.m_sEventType = "EVENT_DOWN";
    sInputAction.m_sCode = "KEY_LCTRL";
    vecInputAction.push_back(sInputAction);
    CCore::GetSingletonPtr()->GetActionManager()->SetAction("SaveActionToInput", vecInputAction);
  }
  //CTPSCamera* pTPSCam = dynamic_cast<CTPSCamera*>(m_pCamera);
  //if(pTPSCam) pTPSCam->AddZoom(delta.z * m_Speed);
  //RAYCAST
  if( pActionManager->DoAction("ShootRayCast") )
  {
    CCamera * l_CurrentCamera = CCameraManager::GetSingletonPtr()->GetCurrentCamera();
    if( l_CurrentCamera )
    {
      CPhysicsManager* l_PM = CCore::GetSingletonPtr()->GetPhysicsManager();
      CPhysicUserData* l_PhysicUserData = new CPhysicUserData("RayCast");
      l_PhysicUserData->SetPaint(true);
      CPhysicActor* l_Actor = new CPhysicActor(l_PhysicUserData);
      //If don't want box, you can remove this line
      l_Actor->AddBoxSphape(Math::Vect3f(0.05f, 0.05f, 0.05f), l_CurrentCamera->GetPos(), Math::Vect3f(0,0,0));
      l_Actor->CreateBody(1.0f);

      // Add at the end allways it needs to have a shape
      l_PM->AddPhysicActor(l_Actor);

      l_Actor->SetLinearVelocity( l_CurrentCamera->GetDirection().GetNormalized() * 20.0f );

      SCollisionInfo &l_SCollisionInfo = SCollisionInfo::SCollisionInfo();
      uint32 mask = 1 << ECG_ESCENE;


      //CPhysicUserData* l_PUD = l_PM->RaycastClosestActor(l_CurrentCamera->GetPos(), l_CurrentCamera->GetDirection().GetNormalized(), mask, l_SCollisionInfo);
      CPhysicUserData* l_PUD = l_PM->RaycastClosestActorShoot(l_CurrentCamera->GetPos(), l_CurrentCamera->GetDirection().GetNormalized(), mask, l_SCollisionInfo, 40.0f);

      if(l_PUD)
        std::string l_Object = l_PUD->GetName();
      else
        std::string l_Object = "";
    }
    
  }
  CCamera * l_CurrentCamera = CCameraManager::GetSingletonPtr()->GetCurrentCamera();

  if ( pActionManager->DoAction("DisableCamera") )
  {
      CCamera * l_CurrentCamera = CCameraManager::GetSingletonPtr()->GetCurrentCamera();
      if( l_CurrentCamera )
      {
        if( l_CurrentCamera->GetEnable() )
          l_CurrentCamera->SetEnable(false);
        else
          l_CurrentCamera->SetEnable(true);
      }
  }
  if ( pActionManager->DoAction("DrawActor") )
  {
    Vect2i l_PosMouse = CCore::GetSingletonPtr()->GetInputManager()->GetMouse()->GetPosition();
    Vect3f l_Pos, l_Dir;
    
    CGraphicsManager::GetSingletonPtr()->GetRay(l_PosMouse,l_Pos, l_Dir);
    
    CPhysicsManager* l_PM = CCore::GetSingletonPtr()->GetPhysicsManager();
    
    SCollisionInfo &l_SCollisionInfo = SCollisionInfo::SCollisionInfo();
    uint32 mask = 1 << ECG_ESCENE;
    
	
	  CPhysicUserData* l_PUD = l_PM->RaycastClosestActor(l_CurrentCamera->GetPos(), l_CurrentCamera->GetDirection().GetNormalized(), mask, l_SCollisionInfo);
    //CPhysicUserData* l_PUD = l_PM->RaycastClosestActor(l_Pos, l_Dir.GetNormalized(), mask, l_SCollisionInfo);
    if(l_PUD)
    {
      l_PUD->SetColor(colRED);
    }
  }
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode("update()");

  //CCore::GetSingletonPtr()->GetPhysicsManager()->AddGravity(Math::Vect3f(0,1*deltaTime,0));
  p_Grenade->Update();
}

void CTestProcess::Init()
{
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode("init()");
  p_Grenade = new CGrenade( 1.5f, 0.2f, 0.5f, 20.0f, "Grenade" );
  p_Grenade->Start();

  /*CPhysicUserData* l_PUD = new CPhysicUserData("Box");
  l_PUD->SetPaint(true);
  CPhysicActor* l_pPhysicActor = new CPhysicActor(l_PUD);
  l_pPhysicActor->AddBoxSphape(Math::Vect3f(0.2f,0.4f,0.2f),Math::Vect3f(0,0,0),Math::Vect3f(-0.75,10,-0.75));
  l_pPhysicActor->AddBoxSphape(Math::Vect3f(0.2f,0.4f,0.2f),Math::Vect3f(0,0,0),Math::Vect3f(0.75,10,-0.75));
  l_pPhysicActor->AddBoxSphape(Math::Vect3f(0.2f,0.4f,0.2f),Math::Vect3f(0,0,0),Math::Vect3f(-0.75,10,0.75));
  l_pPhysicActor->AddBoxSphape(Math::Vect3f(0.2f,0.4f,0.2f),Math::Vect3f(0,0,0),Math::Vect3f(0.75,10,0.75));
  l_pPhysicActor->AddBoxSphape(Math::Vect3f(1.0f,0.1f,1.0f),Math::Vect3f(0,0,0),Math::Vect3f(0,10.4f,0));
  l_pPhysicActor->CreateBody(0.5f);
  CCore::GetSingletonPtr()->GetPhysicsManager()->AddPhysicActor(l_pPhysicActor);

  l_pPhysicActor = new CPhysicActor(l_PUD);
  l_pPhysicActor->AddSphereShape(0.5f);
  l_pPhysicActor->SetGlobalPosition(Math::Vect3f(1,12,0));
  l_pPhysicActor->CreateBody(0.5f);
  CCore::GetSingletonPtr()->GetPhysicsManager()->AddPhysicActor(l_pPhysicActor);

  l_pPhysicActor = new CPhysicActor(l_PUD);
  l_pPhysicActor->AddBoxSphape(Math::Vect3f(0.2f,0.4f,0.2f));
  l_pPhysicActor->SetGlobalPosition(Math::Vect3f(0,4,0.4));
  CCore::GetSingletonPtr()->GetPhysicsManager()->AddPhysicActor(l_pPhysicActor);

  l_pPhysicActor = new CPhysicActor(l_PUD);
  l_pPhysicActor->AddPlaneShape(Math::Vect3f(0,1,0),0);
  l_pPhysicActor->SetGlobalPosition(Math::Vect3f(1,0,0));
  CCore::GetSingletonPtr()->GetPhysicsManager()->AddPhysicActor(l_pPhysicActor);*/

  /*CPhysicUserData* l_PUD = new CPhysicUserData("Box");
  l_PUD->SetPaint(true);
  CPhysicActor* l_pPhysicActor = new CPhysicActor(l_PUD);
  l_pPhysicActor->AddBoxSphape(Math::Vect3f(1,1,1),Math::Vect3f(0,0,0),Math::Vect3f(0,10,2));
  l_pPhysicActor->AddBoxSphape(Math::Vect3f(1,1,1),Math::Vect3f(0,0,0),Math::Vect3f(0,10,-2));
  l_pPhysicActor->CreateBody(0.1f);
  CCore::GetSingletonPtr()->GetPhysicsManager()->AddPhysicActor(l_pPhysicActor);

  l_pPhysicActor = new CPhysicActor(l_PUD);
  l_pPhysicActor->AddBoxSphape(Math::Vect3f(1,1,1));
  l_pPhysicActor->SetGlobalPosition(Math::Vect3f(0,4,0));
  CCore::GetSingletonPtr()->GetPhysicsManager()->AddPhysicActor(l_pPhysicActor);

  l_pPhysicActor = new CPhysicActor(l_PUD);
  l_pPhysicActor->AddPlaneShape(Math::Vect3f(0,1,0),0);
  l_pPhysicActor->SetGlobalPosition(Math::Vect3f(1,0,0));
  CCore::GetSingletonPtr()->GetPhysicsManager()->AddPhysicActor(l_pPhysicActor);*/

  CPhysicsManager* l_PM = CCore::GetSingletonPtr()->GetPhysicsManager();

  CPhysicUserData* l_PUD = new CPhysicUserData("Plane");
  l_PUD->SetPaint(true);
  CPhysicActor* l_pPhysicActor = new CPhysicActor(l_PUD);
  int id = l_PM->AddMaterial(0, 0, 0);
  l_pPhysicActor->SetMaterialIndex(id);
  l_pPhysicActor->AddPlaneShape(Math::Vect3f(0,1.5f,1),0);
  l_PM->AddPhysicActor(l_pPhysicActor);

  CPhysicUserData* l_PUD1 = new CPhysicUserData("Box");
  l_PUD1->SetPaint(true);
  l_PUD1->SetColor(colBLACK);
  CPhysicActor* l_pPhysicActor1 = new CPhysicActor(l_PUD1);
  l_pPhysicActor1->AddBoxSphape(Math::Vect3f(1,1,1), Math::Vect3f(0,4,0), Math::Vect3f(0,0,0));
  l_pPhysicActor1->CreateBody(1.0f);
  l_PM->AddPhysicActor(l_pPhysicActor1);

  CPhysicUserData* l_PUD2 = new CPhysicUserData("Box2");
  l_PUD2->SetPaint(true);
  CPhysicActor* l_pPhysicActor2 = new CPhysicActor(l_PUD2);
  l_pPhysicActor2->SetMaterialIndex(1);
  l_pPhysicActor2->AddBoxSphape(Math::Vect3f(1,1,1), Math::Vect3f(4,4,0), Math::Vect3f(0,0,0));
  l_pPhysicActor2->CreateBody(1.0f);
  l_PM->AddPhysicActor(l_pPhysicActor2);

  /*CPhysicUserData* l_PUD3 = new CPhysicUserData("Box3");
  l_PUD3->SetPaint(true);
  CPhysicActor* l_pPhysicActor3 = new CPhysicActor(l_PUD3);
  l_pPhysicActor3->AddBoxSphape(Math::Vect3f(1,1,1), Math::Vect3f(0,0,0), Math::Vect3f(0,4,0));
  l_pPhysicActor3->CreateBody(1.0f);
  l_PM->AddPhysicActor(l_pPhysicActor3);

  CPhysicUserData* l_PUD4 = new CPhysicUserData("Box4");
  l_PUD4->SetPaint(true);
  CPhysicActor* l_pPhysicActor4 = new CPhysicActor(l_PUD4);
  l_pPhysicActor4->AddBoxSphape(Math::Vect3f(1,1,1), Math::Vect3f(0,0,0), Math::Vect3f(0,6,0));
  l_pPhysicActor4->CreateBody(1.0f);
  l_PM->AddPhysicActor(l_pPhysicActor4);

  CPhysicUserData* l_PUD5 = new CPhysicUserData("Box5");
  l_PUD5->SetPaint(true);
  CPhysicActor* l_pPhysicActor5 = new CPhysicActor(l_PUD5);
  l_pPhysicActor5->AddBoxSphape(Math::Vect3f(1,1,1), Math::Vect3f(0,0,0), Math::Vect3f(0,8,0));
  l_pPhysicActor5->CreateBody(1.0f);
  l_PM->AddPhysicActor(l_pPhysicActor5);

  CPhysicUserData* l_PUD6 = new CPhysicUserData("Box6");
  l_PUD6->SetPaint(true);
  CPhysicActor* l_pPhysicActor6 = new CPhysicActor(l_PUD6);
  l_pPhysicActor6->AddBoxSphape(Math::Vect3f(1,1,1), Math::Vect3f(0,0,0), Math::Vect3f(0,10,0));
  l_pPhysicActor6->CreateBody(1.0f);
  l_PM->AddPhysicActor(l_pPhysicActor6);*/
  

  /*
  l_pPhysicActor = new CPhysicActor(l_PUD);
  l_pPhysicActor->AddSphereShape(0.2f);
  l_pPhysicActor->SetGlobalPosition(Math::Vect3f(0,1,0));
  l_pPhysicActor->CreateBody(0.1f);
  CCore::GetSingletonPtr()->GetPhysicsManager()->AddPhysicActor(l_pPhysicActor);*/

  /*
    CPhysicUserData* l_PUD = new CPhysicUserData("Box");
    l_PUD->SetPaint(true);
    CPhysicActor* l_pPhysicActor = new CPhysicActor(l_PUD);
    l_pPhysicActor->AddBoxSphape(Math::Vect3f(1.0f,1.0f,1.0f), Math::Vect3f(0,0,0),Math::Vect3f(0,0,0));
    l_pPhysicActor->AddBoxSphape(Math::Vect3f(1.0f,1.0f,1.0f), Math::Vect3f(0,0,0),Math::Vect3f(0,1,0));
    l_pPhysicActor->AddBoxSphape(Math::Vect3f(1.0f,1.0f,1.0f), Math::Vect3f(0,0,0),Math::Vect3f(0,2,0));
    l_pPhysicActor->AddBoxSphape(Math::Vect3f(1.0f,1.0f,1.0f), Math::Vect3f(0,0,0),Math::Vect3f(0,3,0));
    l_pPhysicActor->AddBoxSphape(Math::Vect3f(1.0f,1.0f,1.0f), Math::Vect3f(0,0,0),Math::Vect3f(0,4,0));
    l_pPhysicActor->AddBoxSphape(Math::Vect3f(1.0f,1.0f,1.0f), Math::Vect3f(0,0,0),Math::Vect3f(0,5,0));
    l_pPhysicActor->AddBoxSphape(Math::Vect3f(1.0f,1.0f,1.0f), Math::Vect3f(0,0,0),Math::Vect3f(0,6,0));
    CCore::GetSingletonPtr()->GetPhysicsManager()->AddPhysicActor(l_pPhysicActor);

  CPhysicActor* l_pPhysicActor1 = new CPhysicActor(l_PUD2);
    Math::Vect3f l_FirstPosSphere = CCameraManager::GetSingletonPtr()->GetCurrentCamera()->GetPos();
    l_pPhysicActor->AddSphereShape(0.2f, l_FirstPosSphere, l_FirstPosSphere );
    l_pPhysicActor->CreateBody(0.1f);
    CCore::GetSingletonPtr()->GetPhysicsManager()->AddPhysicActor(l_pPhysicActor1);

    if (CCore::GetSingletonPtr()->GetInputManager()->IsDownUp(IDV_KEYBOARD, KEY_SPACE))
      l_pPhysicActor1->AddImpulseAtPos(CCameraManager::GetSingletonPtr()->GetCurrentCamera()->GetDirection(),l_FirstPosSphere,150,true);*/

}

void CTestProcess::Render()
{
  CGraphicsManager* pGraphicsManager = GraphicsInstance;
  p_Grenade->Render();

  /*pGraphicsManager->DrawAxis(5);
  pGraphicsManager->DrawGrid(100, Math::colORANGE, 50, 50);*/
  //pGraphicsManager->DrawTeapot();

  CCore::GetSingletonPtr()->GetScriptManager()->RunCode("render()");

  // START: TO DELETE LATER IF IS NOT NECESSARY,
  unsigned int v = CGPUStatics::GetSingletonPtr()->GetVertexCount();
  unsigned int f = CGPUStatics::GetSingletonPtr()->GetFacesCount();
  unsigned int d = CGPUStatics::GetSingletonPtr()->GetDrawCount();
  CGPUStatics::GetSingletonPtr()->SetToZero();
  CCore::GetSingletonPtr()->GetFontManager()->DrawDefaultText(300, 0, Math::CColor(0.0f, 0.0f, 0.0f), "Vertex: %u   Faces: %u   Draws:%u", v, f, d);
  // END: TO DELETE LATER IF IS NOT NECESSARY
}

void CTestProcess::RenderDebugInfo()
{
  CProcess::RenderDebugInfo();
}