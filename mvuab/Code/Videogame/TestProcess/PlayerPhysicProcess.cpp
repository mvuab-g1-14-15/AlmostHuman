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
#include "VideoPlayer.h"
#include "GraphicsManager.h"
#include "Lights\LightManager.h"
#include "Cameras\CameraManager.h"
#include "Effects\EffectManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableVertex\IndexedVertexs.h"
#include "SceneRenderComands\SceneRendererCommandManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"

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
  ScriptMInstance->RunCode( "update_gameplay()" );
}

void CPlayerPhysicProcess::Init()
{
  CPhysicsManager* l_PM = PhysXMInstance;
  l_PM->SetTriggerReport( this );

  LOG_INFO_APPLICATION("Process finish init");
}

void CPlayerPhysicProcess::Render()
{
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
    if (LuaCode == "")
        return "";

  std::ostringstream codeCat;
  size_t count = LuaCode.find( ")" );
  size_t count2 = LuaCode.find( "(" );
  std::string l_LuaCode2 = LuaCode.substr( 0, count );

  if ( ( count - count2 ) ==
       1 ) //Si es 1 es que no tiene parametro por defecto, por ejemplo  funcion() y pasaría a function(other_shape)
    codeCat << l_LuaCode2 << "'" << Other_Shape.c_str() << "'" << ")";
  else //en este caso podría ser algo así --> funcion(parametro1, parametro2) y añadir el othershape como tercer parametro
    codeCat << l_LuaCode2 << " ," << "'" << Other_Shape.c_str() << "')";

  return codeCat.str();
}

void CPlayerPhysicProcess::OnEnter( CPhysicUserData* _Entity_Trigger1,
                                    CPhysicUserData* _Other_Shape )
{
  std::string l_Msg = "On Enter de " + _Other_Shape->GetName() + " a " +
                      _Entity_Trigger1->GetName();
  CTrigger* l_Trigger = TriggersMInstance->GetTriggerByName( _Entity_Trigger1->GetName() );

  if (!l_Trigger || !l_Trigger->GetActive())
    return;

  if ( l_Trigger->GetbEnter() && _Other_Shape->GetName() == "Player" )
  {
    //Get method name
    std::string l_LuaCode = l_Trigger->GetLUAByName( l_Trigger->ENTER );
    std::string l_NameShape = _Other_Shape->GetName();
    ScriptMInstance->RunCode( GetLuaCodeComplete( l_LuaCode, l_NameShape ) );
    //LOG_INFO_APPLICATION( l_Msg.c_str() );
  }
}

void CPlayerPhysicProcess::OnLeave( CPhysicUserData* _Entity_Trigger1,
                                    CPhysicUserData* _Other_Shape )
{
  std::string l_Msg = "On Leave de " + _Other_Shape->GetName() + " a " +
                      _Entity_Trigger1->GetName();
  CTrigger* l_Trigger = TriggersMInstance->GetTriggerByName( _Entity_Trigger1->GetName() );

  if (!l_Trigger || !l_Trigger->GetActive())
    return;

  //Get method name
  if ( l_Trigger->GetbLeave() && _Other_Shape->GetName() == "Player" )
  {
    std::string l_LuaCode = l_Trigger->GetLUAByName( CTrigger::LEAVE );
    std::string l_NameShape = _Other_Shape->GetName();
    ScriptMInstance->RunCode( GetLuaCodeComplete( l_LuaCode, l_NameShape ) );
    //LOG_INFO_APPLICATION( l_Msg.c_str() );
  }
}

void CPlayerPhysicProcess::OnStay( CPhysicUserData* _Entity_Trigger1,
                                   CPhysicUserData* _Other_Shape )
{
  std::string l_Msg = "On Stay de " + _Other_Shape->GetName() + " a " +
                      _Entity_Trigger1->GetName();
  CTrigger* l_Trigger = TriggersMInstance->GetTriggerByName( _Entity_Trigger1->GetName() );
  
  if (!l_Trigger || !l_Trigger->GetActive())
    return;

  //Get method name
  if ( l_Trigger->GetbStay() && _Other_Shape->GetName() == "Player" )
  {
    std::string l_LuaCode = l_Trigger->GetLUAByName( CTrigger::STAY );
    std::string l_NameShape = _Other_Shape->GetName();
    ScriptMInstance->RunCode( GetLuaCodeComplete( l_LuaCode, l_NameShape ) );
    //LOG_INFO_APPLICATION( l_Msg.c_str() );
  }
}