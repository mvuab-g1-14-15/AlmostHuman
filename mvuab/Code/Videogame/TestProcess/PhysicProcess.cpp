#include "PhysicProcess.h"
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

#include "..\Items\Grenade.h"
#include "Joints\PhysicSphericalJoint.h"
#include "Joints\PhysicRevoluteJoint.h"
#include "Reports\PhysicTriggerReport.h"
#include "Triggers\TriggerManager.h"
#include "Triggers\Trigger.h"



CGrenade* p_Grenade;

void GetFilesFromPath( const std::string& Path,
                       std::vector<std::string>& _OutFiles );

CPhysicProcess::CPhysicProcess() : CProcess(),
  m_Speed( 0.1f ),
  m_Amount( 0.0f ), m_Angle( 0.0f ),  m_AngleMoon( 0.0f ), m_PaintAll( false )

{
  //CCameraManager::GetSingletonPtr()->NewCamera(CCamera::FirstPerson, "TestProcessCam", Math::Vect3f(-15.0f,2.0f,0.0f),
  //  Math::Vect3f(0.0f,2.0f,0.0f) );
  CCameraManager::GetSingletonPtr()->SetCurrentCamera( "TestProcessCam" );
  unsigned short debug = VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL |
                         VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TEXTURE1 |
                         VERTEX_TYPE_DIFFUSE;
}
bool done = false;

CPhysicProcess::~CPhysicProcess()
{
  CLogger::GetSingletonPtr()->SaveLogsInFile();
  CHECKED_DELETE( p_Grenade );
  CHECKED_DELETE( m_PRJ );
  CHECKED_DELETE( m_PSJ );

  for ( size_t i = 0; i < m_vPA.size(); ++i )
    CHECKED_DELETE( m_vPA[i] );

  m_vPA.clear();

  for ( size_t i = 0; i < m_vPUD.size(); ++i )
    CHECKED_DELETE( m_vPUD[i] );

  m_vPUD.clear();
  CHECKED_DELETE( m_TriggerManager );
}

void CPhysicProcess::Update()
{
  m_Amount +=  0.01f;
  m_Angle  += deltaTime * 0.05f;
  m_AngleMoon += deltaTime * 0.05f;
  CActionManager* pActionManager = CActionManager::GetSingletonPtr();

  if ( pActionManager->DoAction( "ReloadStaticMesh" ) )
    CCore::GetSingletonPtr()->GetStaticMeshManager()->Reload();

  if ( pActionManager->DoAction( "ReloadLUA" ) )
    CCore::GetSingletonPtr()->GetScriptManager()->Reload();

  if ( pActionManager->DoAction( "ReloadShaders" ) )
    CCore::GetSingletonPtr()->GetEffectManager()->Reload();

  if ( pActionManager->DoAction( "ReloadActionToInput" ) )
    CCore::GetSingletonPtr()->GetActionManager()->Reload();

  if ( pActionManager->DoAction( "SaveActionToInput" ) )
    CCore::GetSingletonPtr()->GetActionManager()->SaveXML( "Data/Prueba.xml" );

  if ( pActionManager->DoAction( "SetActionToInput" ) )
  {
    std::vector<S_INPUT_ACTION> vecInputAction;
    S_INPUT_ACTION sInputAction;
    sInputAction.m_DeviceType = IDV_KEYBOARD;
    sInputAction.m_EventType = EVENT_DOWN_UP;
    sInputAction.m_Code = KEY_H;
    sInputAction.m_sDeviceType = "IDV_KEYBOARD";
    sInputAction.m_sEventType = "EVENT_DOWN_UP";
    sInputAction.m_sCode = "KEY_H";
    vecInputAction.push_back( sInputAction );
    sInputAction.m_DeviceType = IDV_KEYBOARD;
    sInputAction.m_EventType = EVENT_DOWN;
    sInputAction.m_Code = KEY_LCTRL;
    sInputAction.m_sDeviceType = "IDV_KEYBOARD";
    sInputAction.m_sEventType = "EVENT_DOWN";
    sInputAction.m_sCode = "KEY_LCTRL";
    vecInputAction.push_back( sInputAction );
    CCore::GetSingletonPtr()->GetActionManager()->SetAction( "SaveActionToInput",
        vecInputAction );
  }

  //CTPSCamera* pTPSCam = dynamic_cast<CTPSCamera*>(m_pCamera);
  //if(pTPSCam) pTPSCam->AddZoom(delta.z * m_Speed);
  //RAYCAST
  if ( pActionManager->DoAction( "LeftMouseButtonPressed" ) )
  {
    CCamera* l_CurrentCamera =
      CCameraManager::GetSingletonPtr()->GetCurrentCamera();

    if ( l_CurrentCamera )
    {
      CPhysicsManager* l_PM = CCore::GetSingletonPtr()->GetPhysicsManager();
      CPhysicUserData* l_PhysicUserData = new CPhysicUserData( "RayCast" );
      l_PhysicUserData->SetPaint( true );
      m_vPUD.push_back( l_PhysicUserData );
      CPhysicActor* l_Actor = new CPhysicActor( l_PhysicUserData );
      //If don't want box, you can remove this line
      l_Actor->AddBoxShape( Math::Vect3f( 0.05f, 0.05f, 0.05f ),
                            l_CurrentCamera->GetPos(), Math::Vect3f( 0, 0, 0 ) );
      l_Actor->CreateBody( 1.0f );
      // Add at the end allways it needs to have a shape
      l_PM->AddPhysicActor( l_Actor );
      m_vPA.push_back( l_Actor );
      l_Actor->SetLinearVelocity( l_CurrentCamera->GetDirection().GetNormalized() *
                                  20.0f );
      SCollisionInfo& l_SCollisionInfo = SCollisionInfo::SCollisionInfo();
      uint32 mask = 1 << ECG_ESCENE;
      //CPhysicUserData* l_PUD = l_PM->RaycastClosestActor(l_CurrentCamera->GetPos(), l_CurrentCamera->GetDirection().GetNormalized(), mask, l_SCollisionInfo);
      CPhysicUserData* l_PUD = l_PM->RaycastClosestActorShoot(
                                 l_CurrentCamera->GetPos(), l_CurrentCamera->GetDirection().GetNormalized(),
                                 mask, l_SCollisionInfo, 40.0f );

      if ( l_PUD )
        std::string l_Object = l_PUD->GetName();
      else
        std::string l_Object = "";
    }
  }

  CCamera* l_CurrentCamera =
    CCameraManager::GetSingletonPtr()->GetCurrentCamera();

  //Enable or disable Current camera
  if ( pActionManager->DoAction( "DisableCamera" ) )
  {
    CCamera* l_CurrentCamera =
      CCameraManager::GetSingletonPtr()->GetCurrentCamera();

    if ( l_CurrentCamera )
    {
      if ( l_CurrentCamera->GetEnable() )
        l_CurrentCamera->SetEnable( false );
      else
        l_CurrentCamera->SetEnable( true );
    }
  }

  //Draw actor with color red
  if ( pActionManager->DoAction( "DrawActor" ) )
  {
    Vect2i l_PosMouse =
      CCore::GetSingletonPtr()->GetInputManager()->GetMouse()->GetPosition();
    Vect3f l_Pos, l_Dir;
    CGraphicsManager::GetSingletonPtr()->GetRay( l_PosMouse, l_Pos, l_Dir );
    CPhysicsManager* l_PM = CCore::GetSingletonPtr()->GetPhysicsManager();
    SCollisionInfo& l_SCollisionInfo = SCollisionInfo::SCollisionInfo();
    uint32 mask = 1 << ECG_ESCENE;
    CPhysicUserData* l_PUD = l_PM->RaycastClosestActor( l_CurrentCamera->GetPos(),
                             l_CurrentCamera->GetDirection().GetNormalized(), mask, l_SCollisionInfo );

    //CPhysicUserData* l_PUD = l_PM->RaycastClosestActor(l_Pos, l_Dir.GetNormalized(), mask, l_SCollisionInfo);
    if ( l_PUD )
      l_PUD->SetColor( colRED );
  }

  if ( pActionManager->DoAction( "LeftBridge" ) )
    m_PRJ->ActiveMotor( -200 );

  if ( pActionManager->DoAction( "RightBridge" ) )
    m_PRJ->ActiveMotor( 200 );

  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "update()" );
  //CCore::GetSingletonPtr()->GetPhysicsManager()->AddGravity(Math::Vect3f(0,1*deltaTime,0));
  p_Grenade->Update();
}

void CPhysicProcess::Init()
{
  m_TriggerManager = new CTriggerManager();
  m_TriggerManager->LoadXML( "Data/triggers.xml" );
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "init()" );
  p_Grenade = new CGrenade( 1.5f, 0.2f, 0.5f, 20.0f, "Grenade" );
  p_Grenade->Start();
  //Ejercicio 1 - Pendulo
  CPhysicsManager* l_PM = CCore::GetSingletonPtr()->GetPhysicsManager();
  m_PSJ = new CPhysicSphericalJoint();
  CPhysicUserData* l_PUD = new CPhysicUserData( "Box" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colBLACK );
  m_vPUD.push_back( l_PUD );
  CPhysicActor* l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddSphereShape( 0.4f, Math::Vect3f( 10, 4, 6 ) );
  l_pPhysicActor->CreateBody( 1.0f );
  m_vPA.push_back( l_pPhysicActor );
  l_PM->AddPhysicActor( l_pPhysicActor );
  l_pPhysicActor->AddVelocityAtPos( Math::Vect3f( 10, 0, 6 ), Math::Vect3f( 10, 4,
                                    6 ), 10 );
  m_PSJ->SetInfo( Math::Vect3f( 5, 4, 6 ), l_pPhysicActor );
  l_PM->AddPhysicSphericalJoint( m_PSJ );
  //Ejercicio 2 - Puente levadiso
  l_PUD = new CPhysicUserData( "Box2" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colBLACK );
  m_vPUD.push_back( l_PUD );
  l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 0.2f, 4, 1 ), Math::Vect3f( 0, 4,
                               0 ) );
  l_pPhysicActor->CreateBody( 1.0f );
  l_PM->AddPhysicActor( l_pPhysicActor );
  m_vPA.push_back( l_pPhysicActor );
  m_PRJ = new CPhysicRevoluteJoint();
  m_PRJ->SetInfo( Math::Vect3f( 0, 0, -1 ), Math::Vect3f( 0, 0, 0 ),
                  l_pPhysicActor );
  m_PRJ->SetMotor( 20, 5.f );
  l_PM->AddPhysicRevoluteJoint( m_PRJ );
  l_PUD = new CPhysicUserData( "Box3" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colBLACK );
  m_vPUD.push_back( l_PUD );
  l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 1, 1, 1 ), Math::Vect3f( -7, -1.5f,
                               0 ) );
  m_vPA.push_back( l_pPhysicActor );
  l_PM->AddPhysicActor( l_pPhysicActor );
  l_PUD = new CPhysicUserData( "Box4" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colBLACK );
  m_vPUD.push_back( l_PUD );
  l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 1, 1, 1 ), Math::Vect3f( 7, -1.5f,
                               0 ) );
  m_vPA.push_back( l_pPhysicActor );
  l_PM->AddPhysicActor( l_pPhysicActor );
  //Asigno el TriggerReport en el physicManager a esta clase
  l_PM->SetTriggerReport( this );
}

void CPhysicProcess::Render()
{
  CGraphicsManager* pGraphicsManager = GraphicsInstance;
  p_Grenade->Render();
  /*pGraphicsManager->DrawAxis(5);
  pGraphicsManager->DrawGrid(100, Math::colORANGE, 50, 50);*/
  //pGraphicsManager->DrawTeapot();
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "render()" );
  // START: TO DELETE LATER IF IS NOT NECESSARY,
  unsigned int v = CGPUStatics::GetSingletonPtr()->GetVertexCount();
  unsigned int f = CGPUStatics::GetSingletonPtr()->GetFacesCount();
  unsigned int d = CGPUStatics::GetSingletonPtr()->GetDrawCount();
  CGPUStatics::GetSingletonPtr()->SetToZero();
  CCore::GetSingletonPtr()->GetFontManager()->DrawDefaultText( 300, 0,
      Math::CColor( 0.0f, 0.0f, 0.0f ), "Vertex: %u   Faces: %u   Draws:%u", v, f,
      d );
  // END: TO DELETE LATER IF IS NOT NECESSARY
}

void CPhysicProcess::RenderDebugInfo()
{
  CProcess::RenderDebugInfo();
}

void CPhysicProcess::OnEnter( CPhysicUserData* _Entity_Trigger1,
                              CPhysicUserData* _Other_Shape )
{
  std::string l_Msg = "On Enter de " + _Other_Shape->GetName() + " a " +
                      _Entity_Trigger1->GetName();
  CTrigger* l_Trigger = m_TriggerManager->GetTriggerByName(
                          _Entity_Trigger1->GetName() );
  std::string l_TriggerEvent = l_Trigger->GetEnter();

  if ( l_TriggerEvent != "" )
  {
    //Hacer lo que deba hacer el script
  }

  CLogger::GetSingletonPtr()->AddNewLog( ELL_INFORMATION, l_Msg.c_str() );
}
void CPhysicProcess::OnLeave( CPhysicUserData* _Entity_Trigger1,
                              CPhysicUserData* _Other_Shape )
{
  std::string l_Msg = "On Leave de " + _Other_Shape->GetName() + " a " +
                      _Entity_Trigger1->GetName();
  CTrigger* l_Trigger = m_TriggerManager->GetTriggerByName(
                          _Entity_Trigger1->GetName() );
  std::string l_TriggerEvent = l_Trigger->GetLeave();

  if ( l_TriggerEvent != "" )
  {
    //Hacer lo que deba hacer el script
  }

  CLogger::GetSingletonPtr()->AddNewLog( ELL_INFORMATION, l_Msg.c_str() );
}
void CPhysicProcess::OnStay( CPhysicUserData* _Entity_Trigger1,
                             CPhysicUserData* _Other_Shape )
{
  std::string l_Msg = "On Stay de " + _Other_Shape->GetName() + " a " +
                      _Entity_Trigger1->GetName();
  CTrigger* l_Trigger = m_TriggerManager->GetTriggerByName(
                          _Entity_Trigger1->GetName() );
  std::string l_TriggerEvent = l_Trigger->GetStay();

  if ( l_TriggerEvent != "" )
  {
    //Hacer lo que deba hacer el script
  }

  CLogger::GetSingletonPtr()->AddNewLog( ELL_INFORMATION, l_Msg.c_str() );
}