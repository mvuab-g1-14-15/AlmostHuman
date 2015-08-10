#include "PlayerPhysicProcess.h"

//AI
#include "Graph\Graph.h"
#include "Characters\Character.h"
#include "Pathfinding\AStar.h"

//BASE
#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\BaseUtils.h"

#include "Logger\Logger.h"
#include "Utils\Defines.h"
#include "Fonts\FontManager.h"
#include "ScriptManager.h"
#include "Utils\Manager.h"

//CORE

#include "EngineConfig.h"
#include "EngineManagers.h"

//GRAPHICS
#include "GraphicsManager.h"
#include "Lights\LightManager.h"
#include "Cameras\CameraManager.h"
#include "Effects\EffectManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableVertex\IndexedVertexs.h"
#include "SceneRenderComands\SceneRendererCommandManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "Billboard\Billboard.h"

//INPUTS
#include "InputManager.h"
#include "ActionManager.h"

//PHYSX
#include "PhysicsManager.h"
#include "Actor\PhysicActor.h"
#include "Utils/PhysicUserData.h"
#include "Actor\PhysicController.h"
#include "Triggers\Trigger.h"
#include "Triggers\TriggerManager.h"
#include "CookingMesh\PhysicCookingMesh.h"

//SOUND
#include "SoundManager.h"

//GUI
#include "GUIManager.h"
#include "Widgets\ConsoleGUI.h"


#include <algorithm>
#include "RenderableVertex\VertexTypes.h"

CPlayerPhysicProcess::CPlayerPhysicProcess() : CProcess(), m_ConsoleActivate( false )
{
  unsigned short i = VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_INDICES | VERTEX_TYPE_WEIGHT | VERTEX_TYPE_NORMAL |
                     VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL | VERTEX_TYPE_TEXTURE1;
}

CPlayerPhysicProcess::~CPlayerPhysicProcess()
{
  CLogger::GetSingletonPtr()->SaveLogsInFile();

  for ( size_t i = 0; i < m_vPA.size(); ++i )
    CHECKED_DELETE( m_vPA[i] );

  m_vPA.clear();

  for ( size_t i = 0; i < m_vPUD.size(); ++i )
    CHECKED_DELETE( m_vPUD[i] );

  m_vPUD.clear();

  for ( size_t i = 0; i < m_vController.size(); ++i )
    CHECKED_DELETE( m_vController[i] );

  m_vController.clear();
}

void CPlayerPhysicProcess::Update()
{
  /*
      CGraph l_Graph;
      l_Graph.Parse("./Data/graph1.xml");

      unsigned int a = l_Graph.GetArcWeight(0, 3);
      unsigned int b = l_Graph.GetArcWeight(4, 1);

      Math::Vect3f v = l_Graph.GetNodeInfo(4);
  */
  // m_ConsoleActivate = GUIInstance->GetConsole()->GetVisible();

  //ScriptMInstance->RunCode( "update()" );
  ScriptMInstance->RunCode( "update_gameplay()" );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
////////////        INICIALIZACIÓN DE LA ESCENA PARA EL TEST DEL CHARACTER CONTROLLER      ///////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
void CPlayerPhysicProcess::InitSceneCharacterController()
{
  /*
      //Scene Character Controller
      //Step1
      std::string l_Type = "Box";
      PhysXMInstance->AddActor( "BoxCharacter1", l_Type, Math::Vect3f( 2, 1, 2 ), colWHITE, true, Math::Vect3f( 0, 20, 5 ),
                          v3fZERO,
                          v3fZERO, 0, 0 );

      //Step2
      l_Type = "Box";
      PhysXMInstance->AddActor( "BoxCharacter2", l_Type, Math::Vect3f( 2, 2, 2 ), colWHITE, true, Math::Vect3f( 0, 21, 5 ),
                          Math::Vect3f( 4, 0, 0 ), v3fZERO, 0, 0 );

      //Step3
      l_Type = "Box";
      PhysXMInstance->AddActor( "BoxCharacter3", l_Type, Math::Vect3f( 2, 3, 2 ), colWHITE, true, Math::Vect3f( 0, 22, 5 ),
                          Math::Vect3f( 8, 0, 0 ), v3fZERO, 0, 0 );

      //Plano Inclinado TODO
      l_Type = "Box";
      PhysXMInstance->AddActor( "Rampa", l_Type, Math::Vect3f( 0.5f, 10, 4 ), colWHITE, true, Math::Vect3f( 0, 20, -5 ),
                          Math::Vect3f( 3, 0, 0 ), Math::Vect3f( 0, 0, 1.3f ), 0, 0 );
  */

}

void CPlayerPhysicProcess::Init()
{
  //ScriptMInstance->RunCode( "init()" );
  ScriptMInstance->RunCode( "load_gameplay()" );
  CPhysicsManager* l_PM = PhysXMInstance;
  l_PM->SetTriggerReport( this );

  /*  CWWSoundManager* l_SM = SoundMan;

      uint32 l_source1 =  l_SM->CreateSource();
      l_SM->SetSourcePosition( l_source1, Math::Vect3f( 2.0 ) );
      l_SM->SetSourceGain( l_source1, 100.0f );

      uint32 l_source2 =  l_SM->CreateSource();
      l_SM->SetSourcePosition( l_source2, Math::Vect3f( 5.0 ) );
      l_SM->SetSourceGain( l_source2, 100.0f );

      uint32 l_source3 =  l_SM->CreateSource();
      l_SM->SetSourcePosition( l_source3, Math::Vect3f( 5.0 ) );
      l_SM->SetSourceGain( l_source3, 100.0f );

      l_SM->SetListenerPosition( Math::Vect3f( 0.0 ) );
      l_SM->SetListenerOrientation( CameraMInstance->GetCurrentCamera()->GetDirection(),
                                CameraMInstance->GetCurrentCamera()->GetVecUp() );
  */
  //l_SM->PlayAction2D( "test" );
  //l_SM->PlaySource2D( l_source3 , "test", true );

  //CPhysicUserData* l_pPhysicUserDataASEMesh;
  //CPhysicActor*  l_AseMeshActor;

  //CPhysicCookingMesh* l_pMeshes = PhysXMInstance->GetCookingMesh();
  //CStaticMeshManager* l_StaticMeshManager = SMeshMInstance;

  //if ( l_pMeshes->CreateMeshFromASE( "Data/a.ASE", "Escenario" ) )
  //{
  //  l_pPhysicUserDataASEMesh = new CPhysicUserData( "Escenario" );
  //  l_pPhysicUserDataASEMesh->SetColor( Math::colBLACK );
  //  m_vPUD.push_back( l_pPhysicUserDataASEMesh );
  //  l_AseMeshActor = new CPhysicActor( l_pPhysicUserDataASEMesh );

  //  VecMeshes l_CookMeshes = l_pMeshes->GetMeshes();

  //  for ( VecMeshes::iterator it = l_CookMeshes.begin(); it != l_CookMeshes.end(); it++ )
  //    l_AseMeshActor->AddMeshShape( it->second, Vect3f( 0, 0, 0 ) );

  //  //m_AseMeshActor->CreateBody ( 10.f );
  //  PhysXMInstance->AddPhysicActor( l_AseMeshActor );
  //  m_vPA.push_back( l_AseMeshActor );
  //}

  //Add Escenario

  /*
      CRenderableObjectsLayersManager *l_ROLM = CEngineManagers::GetSingletonPtr()->GetROLManager();
      l_ROLM->GetResourcesVector()[0]-;
      std::map<std::string, CStaticMesh*> &l_MapResources = SMeshMInstance->GetResourcesMap();
      std::map<std::string, CStaticMesh*>::iterator l_itBegin = l_MapResources.begin();
      std::map<std::string, CStaticMesh*>::iterator l_itEnd = l_MapResources.end();



      while(l_itBegin != l_itEnd)
      {
        PhysXMInstance->GetCookingMesh()->CreatePhysicMesh(l_itBegin->first, l_itBegin->second->GetVB(), l_itBegin->second->GetIB());
        l_itBegin++;
      }

      VecMeshes l_VecMeshes = PhysXMInstance->GetCookingMesh()->GetMeshes();
      for (VecMeshes::iterator it = l_VecMeshes.begin(); it != l_VecMeshes.end(); it++)
      {
        CPhysicUserData* l_pPhysicUserDataASEMesh = new CPhysicUserData(it->first);
        CPhysicActor* l_AseMeshActor = new CPhysicActor(l_pPhysicUserDataASEMesh);

        l_AseMeshActor->AddMeshShape(it->second, Vect3f(0, 0, 0));
        bool oK = false;

        if(PhysXMInstance->CMapManager<CPhysicActor>::GetResource(it->first) == 0)
        {
            if(PhysXMInstance->AddPhysicActor(l_AseMeshActor))
            {
                PhysXMInstance->CMapManager<CPhysicActor>::AddResource(it->first, l_AseMeshActor);
                oK = TRUE;
            }
        }

        if(!oK)
        {
            CHECKED_DELETE(l_AseMeshActor);
            CHECKED_DELETE(l_pPhysicUserDataASEMesh);
        }
      }
  */

  /*  if ( !PhysXMInstance->AddMesh( "Data/a.ASE", "Escenario" ) )
      LOG_ERROR_APPLICATION( "CPlayerPhysicProcess::Init No se pudo crear la malla Escenario!" );*/

  m_PointInicial = Math::Vect3f( 6, 0, -6 );
  m_PointFinal = Math::Vect3f( 6, 0, 6 );
  InitSceneCharacterController();
}

void CPlayerPhysicProcess::Render()
{
  //m_Grenade->Render();
  //  m_AStar->Render();
  //m_Billboard->Render();

  //    m_Blaster->Render();
}

void CPlayerPhysicProcess::RenderDebugInfo()
{
  CProcess::RenderDebugInfo();
}

CPhysicUserData* CPlayerPhysicProcess::GetNewPUD( const std::string& Name )
{
  m_vPUD.push_back( new CPhysicUserData( Name ) );
  return m_vPUD[m_vPUD.size() - 1];
}
CPhysicActor* CPlayerPhysicProcess::GetNewPhysicActor( CPhysicUserData* PUD )
{
  m_vPA.push_back( new CPhysicActor( PUD ) );
  return m_vPA[m_vPA.size() - 1];
}

void CPlayerPhysicProcess::AddPudVector( CPhysicUserData* PUD )
{
  m_vPUD.push_back( PUD );
}
void CPlayerPhysicProcess::AddPhysicActorVector( CPhysicActor* PA )
{
  m_vPA.push_back( PA );
}
CPhysicUserData* CPlayerPhysicProcess::GetLastPUDInserted()
{
  return m_vPUD[m_vPUD.size() - 1];
}

CPhysicController*  CPlayerPhysicProcess::GetNewController( float _fRadius, float _fHeight, float _fSlope,
    float _fSkinwidth, float _fStepOffset,
    CPhysicUserData* _pUserData, const Math::Vect3f& _vPos, float _fGravity )
{
  m_vController.push_back( new CPhysicController( _fRadius, _fHeight, _fSlope, _fSkinwidth, _fStepOffset, ECG_PLAYER,
                           _pUserData, _vPos, _fGravity ) );
  return m_vController[ m_vController.size() - 1 ];
}

void CPlayerPhysicProcess::DeleteController( CPhysicUserData* PUD )
{
  std::vector<CPhysicController*>::iterator it = std::find( m_vController.begin(), m_vController.end(),
      PUD->GetController() );

  if ( it != m_vController.end() )
  {
    CHECKED_DELETE( *it );
    std::vector<CPhysicUserData*>::iterator itPUD = std::find( m_vPUD.begin(), m_vPUD.end(), PUD );

    if ( itPUD != m_vPUD.end() )
      CHECKED_DELETE( *itPUD );
  }
}

//////////////////////////////////////////////////////////////////////////////////////
////////////   Método para pasarle el nombre del shape por parametro     /////////////
//////////////////////////////////////////////////////////////////////////////////////
std::string GetLuaCodeComplete( std::string LuaCode, std::string Other_Shape )
{
  std::ostringstream codeCat;
  size_t count = LuaCode.find( ")" );
  size_t count2 = LuaCode.find( "(" );
  std::string l_LuaCode2 = LuaCode.substr( 0, count );

  if ( ( count - count2 ) ==
       1 ) //Si es 1 es que no tiene parametro por defecto, por ejemplo  funcion() y pasaría a function(other_shape)
    codeCat << l_LuaCode2 << "'" << Other_Shape.c_str() << "'" << ")";
  else //en este caso podría ser algo así --> funcion(parametro1, parametro2) y añadir el othershape como tercer parametro
    codeCat << l_LuaCode2 << "," << "'" << Other_Shape.c_str() << "')";

  return codeCat.str();
}
void CPlayerPhysicProcess::OnEnter( CPhysicUserData* _Entity_Trigger1,
                                    CPhysicUserData* _Other_Shape )
{
  std::string l_Msg = "On Enter de " + _Other_Shape->GetName() + " a " +
                      _Entity_Trigger1->GetName();
  CTrigger* l_Trigger = TriggersMInstance->GetTriggerByName( _Entity_Trigger1->GetName() );

  if ( l_Trigger->GetbEnter() && _Other_Shape->GetName() == "Player" )
  {
    //Get method name
    std::string l_LuaCode = l_Trigger->GetLUAByName( l_Trigger->ENTER );
    std::string l_NameShape = _Other_Shape->GetName();
    ScriptMInstance->RunCode( GetLuaCodeComplete( l_LuaCode, l_NameShape ) );
    LOG_INFO_APPLICATION( l_Msg.c_str() );
  }

}
void CPlayerPhysicProcess::OnLeave( CPhysicUserData* _Entity_Trigger1,
                                    CPhysicUserData* _Other_Shape )
{
  std::string l_Msg = "On Leave de " + _Other_Shape->GetName() + " a " +
                      _Entity_Trigger1->GetName();
  CTrigger* l_Trigger = TriggersMInstance->GetTriggerByName( _Entity_Trigger1->GetName() );

  //Get method name
  if ( l_Trigger->GetbLeave() )
  {
    std::string l_LuaCode = l_Trigger->GetLUAByName( CTrigger::LEAVE );
    std::string l_NameShape = _Other_Shape->GetName();
    ScriptMInstance->RunCode( GetLuaCodeComplete( l_LuaCode, l_NameShape ) );
    LOG_INFO_APPLICATION( l_Msg.c_str() );
  }

}
void CPlayerPhysicProcess::OnStay( CPhysicUserData* _Entity_Trigger1,
                                   CPhysicUserData* _Other_Shape )
{
  std::string l_Msg = "On Stay de " + _Other_Shape->GetName() + " a " +
                      _Entity_Trigger1->GetName();
  CTrigger* l_Trigger = TriggersMInstance->GetTriggerByName( _Entity_Trigger1->GetName() );
  //Get method name

  if ( l_Trigger->GetbStay() )
  {
    std::string l_LuaCode = l_Trigger->GetLUAByName( CTrigger::STAY );
    std::string l_NameShape = _Other_Shape->GetName();
    ScriptMInstance->RunCode( GetLuaCodeComplete( l_LuaCode, l_NameShape ) );
    LOG_INFO_APPLICATION( l_Msg.c_str() );
  }

}