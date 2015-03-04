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

#include "Items\Grenade.h"
#include "Joints\PhysicSphericalJoint.h"
#include "Joints\PhysicRevoluteJoint.h"
#include "Reports\PhysicTriggerReport.h"
#include "Triggers\TriggerManager.h"
#include "Triggers\Trigger.h"
#include "Lights\LightManager.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "Actor\PhysicController.h"
#include "Character.h"

#include <d3dx9.h>
#include "SceneRenderComands\SceneRendererCommandManager.h"

#include "StateMachine\StateMachine.h"

CGrenade* p_Grenade;

void GetFilesFromPath( const std::string& Path,
                       std::vector<std::string>& _OutFiles );

CPhysicProcess::CPhysicProcess() : CProcess(),
  m_Speed( 0.1f ),
  m_Amount( 0.0f ), m_Angle( 0.0f ),  m_AngleMoon( 0.0f ), m_PaintAll( false ),
  m_Salir( false ), m_Time( 0 )

{
  //CCameraManager::GetSingletonPtr()->NewCamera(CCamera::FirstPerson, "TestProcessCam", Math::Vect3f(-15.0f,2.0f,0.0f),
  //  Math::Vect3f(0.0f,2.0f,0.0f) );
  //CCameraManager::GetSingletonPtr()->SetCurrentCamera( "TestProcessCam" );
}
bool done = false;

CPhysicProcess::~CPhysicProcess()
{
  CLogger::GetSingletonPtr()->SaveLogsInFile();
  CHECKED_DELETE( p_Grenade );
  //CHECKED_DELETE( m_PRJ );
  //CHECKED_DELETE( m_PSJ );

  for ( size_t i = 0; i < m_vPA.size(); ++i )
    CHECKED_DELETE( m_vPA[i] );

  m_vPA.clear();

  for ( size_t i = 0; i < m_vPUD.size(); ++i )
    CHECKED_DELETE( m_vPUD[i] );

  m_vPUD.clear();

  for ( size_t i = 0; i < m_vCharacter.size(); ++i )
    CHECKED_DELETE( m_vCharacter[i] );

  m_vCharacter.clear();
  CHECKED_DELETE( m_TriggerManager );
  /*CHECKED_DELETE( m_pPUD );
  CHECKED_DELETE( m_pPhysicActor );*/
  CHECKED_DELETE( m_PhysicController );
  //CHECKED_DELETE( m_Character );
}

void CPhysicProcess::Update()
{
  //m_Character->Update();
  m_Amount +=  0.01f;
  m_Angle  += deltaTime * 0.05f;
  m_AngleMoon += deltaTime * 0.05f;
  CActionManager* pActionManager = CActionManager::GetSingletonPtr();

  if ( pActionManager->DoAction( "ReloadStaticMesh" ) )
    CCore::GetSingletonPtr()->GetStaticMeshManager()->Reload();

  if ( pActionManager->DoAction( "ReloadLUA" ) )
    CCore::GetSingletonPtr()->GetScriptManager()->Reload();

  if ( pActionManager->DoAction( "ReloadShaders" ) )
  {
    // NOTE this must be in this order
    CEffectManager::GetSingletonPtr()->Reload();
    CLightManager::GetSingletonPtr()->ReLoad();
    CRenderableObjectTechniqueManager::GetSingletonPtr()->ReLoad();
    CStaticMeshManager::GetSingletonPtr()->Reload();
    CRenderableObjectsLayersManager::GetSingletonPtr()->Reload();
    CSceneRendererCommandManager::GetSingletonPtr()->ReLoad();
  }

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
      l_Actor->AddBoxShapeHardcoded( Math::Vect3f( 0.05f, 0.05f, 0.05f ),
                                     l_CurrentCamera->GetPos(), Math::Vect3f( 0, 0, 0 ), Math::Vect3f( 0, 0, 0 ) );
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

  /* Math::Vect3f l_Direction = Math::Vect3f( 0.0f, 0.0f, 0.0f );
   float  l_Yaw = CCameraManager::GetSingletonPtr()->GetCurrentCamera()->GetYaw();

   if ( pActionManager->DoAction( "MoveForward" ) )
     l_Direction += Math::Vect3f( Math::Utils::Cos( l_Yaw ), 0.0f, Math::Utils::Sin( l_Yaw ) );

   if ( pActionManager->DoAction( "MoveBackward" ) )
     l_Direction -= Math::Vect3f( Math::Utils::Cos( l_Yaw ), 0.0f, Math::Utils::Sin( l_Yaw ) );

   if ( pActionManager->DoAction( "MoveLeft" ) )
     l_Direction += Math::Vect3f( Math::Utils::Cos( l_Yaw + Math::pi32 / 2 ), 0.0f,
                                  Math::Utils::Sin( l_Yaw + Math::pi32 / 2 ) );

   if ( pActionManager->DoAction( "MoveRight" ) )
     l_Direction -= Math::Vect3f( Math::Utils::Cos( l_Yaw + Math::pi32 / 2 ), 0.0f,
                                  Math::Utils::Sin( l_Yaw + Math::pi32 / 2 ) );

   if ( l_Direction != Math::Vect3f( 0.0f, 0.0f, 0.0f ) )
     l_Direction = l_Direction.GetNormalized();

   m_PhysicController->Move( l_Direction * 0.05f, deltaTime );

   if ( pActionManager->DoAction( "Jump" ) )
     m_PhysicController->Jump( 50 );*/
  //TODO CHARACTERCONTROLLER Descomentar para que la camara siga al character controller
  //CCameraManager::GetSingletonPtr()->GetCurrentCamera()->SetPos( m_PhysicController->GetPosition() );
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "update()" );
  //CCore::GetSingletonPtr()->GetPhysicsManager()->AddGravity(Math::Vect3f(0,1*deltaTime,0));
  p_Grenade->Update();

  if ( m_Salir && ( m_Time >= 1 ) )
  {
    CCore::GetSingletonPtr()->GetPhysicsManager()->ReleasePhysicActor( m_vPA[m_vPA.size() - 1] );
    m_Salir = false;
    m_Time = 0;
  }
  else if ( m_Salir )
    m_Time += deltaTime;
}

void CPhysicProcess::InitSceneCharacterController()
{
  //Scene Character Controller
  //Step1
  CPhysicUserData* l_PUD = new CPhysicUserData( "BoxCharacter1" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colWHITE );
  m_vPUD.push_back( l_PUD );
  CPhysicActor* l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 2, 1, 2 ), Math::Vect3f( 0, 0, 5 ), Math::Vect3f( 0, 0,
                               0 ) );
  CPhysicsManager::GetSingletonPtr()->AddPhysicActor( l_pPhysicActor );
  m_vPA.push_back( l_pPhysicActor );
  //Step2
  l_PUD = new CPhysicUserData( "BoxCharacter2" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colWHITE );
  m_vPUD.push_back( l_PUD );
  l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 2, 2, 2 ), Math::Vect3f( 0, 0, 5 ), Math::Vect3f( 4, 0,
                               0 ) );
  CPhysicsManager::GetSingletonPtr()->AddPhysicActor( l_pPhysicActor );
  m_vPA.push_back( l_pPhysicActor );
  //Step3
  l_PUD = new CPhysicUserData( "BoxCharacter3" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colWHITE );
  m_vPUD.push_back( l_PUD );
  l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 2, 3, 2 ), Math::Vect3f( 0, 0, 5 ), Math::Vect3f( 8, 0,
                               0 ) );
  CPhysicsManager::GetSingletonPtr()->AddPhysicActor( l_pPhysicActor );
  m_vPA.push_back( l_pPhysicActor );
  //Plano Inclinado TODO
  l_PUD = new CPhysicUserData( "Rampa" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colWHITE );
  m_vPUD.push_back( l_PUD );
  l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 0.5f, 10, 4 ), Math::Vect3f( 0, 0, -5 ), Math::Vect3f( 3,
                               0, 0 ),
                               Math::Vect3f( 0, 0, 1.3f ) );
  CPhysicsManager::GetSingletonPtr()->AddPhysicActor( l_pPhysicActor );
  m_vPA.push_back( l_pPhysicActor );
}

void CPhysicProcess::InitScenePhysicsSamplers()
{
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
  l_PUD->SetColor( colWHITE );
  m_vPUD.push_back( l_PUD );
  l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 0.2f, 4, 1 ), Math::Vect3f( 0, 0, 0 ), Math::Vect3f( 0,
                               4, 0 ) );
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
  l_PUD->SetColor( 1, 0, 0, 1 );
  m_vPUD.push_back( l_PUD );
  l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 1, 1, 1 ), Math::Vect3f( -7, 0,
                               0 ) );
  m_vPA.push_back( l_pPhysicActor );
  l_PM->AddPhysicActor( l_pPhysicActor );
  l_PUD = new CPhysicUserData( "Box4" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colBLACK );
  m_vPUD.push_back( l_PUD );
  l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 1, 1, 1 ), Math::Vect3f( 7, 0,
                               0 ) );
  m_vPA.push_back( l_pPhysicActor );
  l_PM->AddPhysicActor( l_pPhysicActor );
  //Asigno el TriggerReport en el physicManager a esta clase
  l_PM->SetTriggerReport( this );
}

void CPhysicProcess::Init()
{
  m_TriggerManager = new CTriggerManager();
  m_TriggerManager->LoadXML( "Data/triggers.xml" );
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( "init()" );
  p_Grenade = new CGrenade( 1.5f, 0.2f, 0.5f, 20.0f, "Grenade" );
  p_Grenade->Start();
  //Create CharacterController
  CPhysicUserData* userData = new CPhysicUserData( "CharacterController" );
  userData->SetPaint( true );
  userData->SetColor( colWHITE );
  m_vPUD.push_back( userData );
  Math::Vect3f l_Pos = CCameraManager::GetSingletonPtr()->GetCurrentCamera()->GetPos();
  m_PhysicController = new CPhysicController( 0.5f, 2, 0.2f, 0.5f, 0.5f, ECG_PLAYER,
      userData, l_Pos );
  CPhysicsManager* l_PM = CCore::GetSingletonPtr()->GetPhysicsManager();
  l_PM->AddPhysicController( m_PhysicController );
  /* m_Character = new CCharacter( "Player" );
   CXMLTreeNode l_node;
   m_Character->Init( l_node );*/
  CPhysicUserData* l_PUD = new CPhysicUserData( "Plane" );
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colMAGENTA );
  m_vPUD.push_back( l_PUD );
  CPhysicActor* l_pPhysicActor = new CPhysicActor( l_PUD );
  l_pPhysicActor->AddBoxShape( Math::Vect3f( 100, 0.2f, 100 ), Math::Vect3f( 0, 0,
                               0 ) );
  m_vPA.push_back( l_pPhysicActor );
  l_PM->AddPhysicActor( l_pPhysicActor );
  //InitSceneCharacterController();

  CStateMachine* l_StateMachine = new CStateMachine("Data/enemies/AI.xml");
  CHECKED_DELETE(l_StateMachine);
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
  //Get method name
  std::string l_LuaCode1 = l_Trigger->GetLUAByName( l_Trigger->ENTER );
  //Parseo para saber si tiene o no argumentos
  std::string l_Other_ShapeName = _Other_Shape->GetName();
  std::ostringstream codeCat;
  size_t count = l_LuaCode1.find( ")" );
  size_t count2 = l_LuaCode1.find( "(" );
  std::string l_LuaCode2 = l_LuaCode1.substr( 0, count );

  if ( ( count - count2 ) == 1 ) //Si es 1 es que no tiene parametro
    codeCat << l_LuaCode2 << "'" << l_Other_ShapeName.c_str() << "'" << ")";
  else
    codeCat << l_LuaCode2 << "," << "'" << l_Other_ShapeName.c_str() << ")";

  std::string l_LuaCode( codeCat.str() );
  //Ejecutar código
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( l_LuaCode );
  CLogger::GetSingletonPtr()->AddNewLog( ELL_INFORMATION, l_Msg.c_str() );
}
void CPhysicProcess::OnLeave( CPhysicUserData* _Entity_Trigger1,
                              CPhysicUserData* _Other_Shape )
{
  std::string l_Msg = "On Leave de " + _Other_Shape->GetName() + " a " +
                      _Entity_Trigger1->GetName();
  CTrigger* l_Trigger = m_TriggerManager->GetTriggerByName(
                          _Entity_Trigger1->GetName() );
  //Get method name
  std::string l_LuaCode1 = l_Trigger->GetLUAByName( CTrigger::LEAVE );
  //Parseo para saber si tiene o no argumentos
  std::string l_Other_ShapeName = _Other_Shape->GetName();
  std::ostringstream codeCat;
  size_t count = l_LuaCode1.find( ")" );
  size_t count2 = l_LuaCode1.find( "(" );
  std::string l_LuaCode2 = l_LuaCode1.substr( 0, count );

  if ( ( count - count2 ) == 1 ) //Si es 1 es que no tiene parametro
    codeCat << l_LuaCode2 << "'" << l_Other_ShapeName.c_str() << "'" << ")";
  else
    codeCat << l_LuaCode2 << "," << "'" << l_Other_ShapeName.c_str() << ")";

  std::string l_LuaCode( codeCat.str() );
  //Ejecutar código
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( l_LuaCode );
  CLogger::GetSingletonPtr()->AddNewLog( ELL_INFORMATION, l_Msg.c_str() );
}
void CPhysicProcess::OnStay( CPhysicUserData* _Entity_Trigger1,
                             CPhysicUserData* _Other_Shape )
{
  std::string l_Msg = "On Stay de " + _Other_Shape->GetName() + " a " +
                      _Entity_Trigger1->GetName();
  CTrigger* l_Trigger = m_TriggerManager->GetTriggerByName(
                          _Entity_Trigger1->GetName() );
  //Get method name
  std::string l_LuaCode1 = l_Trigger->GetLUAByName( CTrigger::STAY );
  //Parseo para saber si tiene o no argumentos
  std::string l_Other_ShapeName = _Other_Shape->GetName();
  std::ostringstream codeCat;
  size_t count = l_LuaCode1.find( ")" );
  size_t count2 = l_LuaCode1.find( "(" );
  std::string l_LuaCode2 = l_LuaCode1.substr( 0, count );

  if ( ( count - count2 ) == 1 ) //Si es 1 es que no tiene parametro
    codeCat << l_LuaCode2 << "'" << l_Other_ShapeName.c_str() << "'" << ")";
  else
    codeCat << l_LuaCode2 << "," << "'" << l_Other_ShapeName.c_str() << ")";

  std::string l_LuaCode( codeCat.str() );
  //Ejecutar código
  CCore::GetSingletonPtr()->GetScriptManager()->RunCode( l_LuaCode );
  CLogger::GetSingletonPtr()->AddNewLog( ELL_INFORMATION, l_Msg.c_str() );
}

CPhysicUserData* CPhysicProcess::GetNewPUD( const std::string& Name )
{
  return new CPhysicUserData( Name );
}
CPhysicActor* CPhysicProcess::GetNewPhysicActor( CPhysicUserData* PUD )
{
  return new CPhysicActor( PUD );
}

void CPhysicProcess::AddPudVector( CPhysicUserData* PUD )
{
  m_vPUD.push_back( PUD );
}
void CPhysicProcess::AddPhysicActorVector( CPhysicActor* PA )
{
  m_vPA.push_back( PA );
}
CPhysicUserData* CPhysicProcess::GetLastPUDInserted()
{
  return m_vPUD[m_vPUD.size() - 1];
}

CPhysicController*  CPhysicProcess::GetNewController( float _fRadius, float _fHeight, float _fSlope,
    float _fSkinwidth, float _fStepOffset,
    ECollisionGroup _uiCollisionGroups, CPhysicUserData* _pUserData, const Math::Vect3f& _vPos,
    float _fGravity )
{
  return new CPhysicController( _fRadius, _fHeight, _fSlope, _fSkinwidth, _fStepOffset,
                                _uiCollisionGroups, _pUserData, _vPos, _fGravity );
}

CCharacter* CPhysicProcess::GetNewCharacter( const std::string& Name )
{
  CCharacter* l_Character = new CCharacter( Name );
  m_vCharacter.push_back( l_Character );
  return l_Character;
}