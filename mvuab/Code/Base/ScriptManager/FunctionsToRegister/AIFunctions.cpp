#include "ScriptManager\FunctionsToRegister\AIFunctions.h"

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "Characters/Character.h"
#include "Characters\Enemies\Enemy.h"
#include "Characters\Enemies\PatrolEnemy.h"
#include "Characters\Enemies\BossEnemy.h"
#include "Characters\Enemies\EasyEnemy.h"
#include "Characters\Enemies\EnemyManager.h"
#include "Utils\MapManager.h"
#include "XML\XMLTreeNode.h"
#include "Object3D.h"


using namespace luabind;

void registerAI( lua_State* m_LS )
{
  module( m_LS )
  [
    class_<CCharacter, CObject3D>( "CCharacter" )
    .def( constructor<const std::string&>() )
    .def( "setTargetPosition", &CCharacter::SetTargetPosition )
    .def( "SetTargetPositionOriginal", &CCharacter::SetTargetPositionOriginal )
    .def( "init", ( void( CCharacter::* )( void ) )&CCharacter::Init )
    .def( "update", &CCharacter::Update )
    .def( "render", &CCharacter::Render )
  ];
  module( m_LS )
  [
    class_<CEnemy, CCharacter>( "CEnemy" )
    .def( "ChangeState", &CEnemy::ChangeState )
  ];

  module( m_LS )
  [
    class_<CPatrolEnemy, CEnemy>( "CPatrolEnemy" )
    .def( constructor<CXMLTreeNode&>() )
    .def( "init", &CPatrolEnemy::Init )
    .def( "update", &CPatrolEnemy::Update )
    .def( "render", &CPatrolEnemy::Render )
  ];
  module( m_LS )
  [
    class_<CBossEnemy, CEnemy>( "CBossEnemy" )
    .def( constructor<CXMLTreeNode&>() )
    .def( "init", &CBossEnemy::Init )
    .def( "update", &CBossEnemy::Update )
    .def( "render", &CBossEnemy::Render )
  ];
  module( m_LS )
  [
    class_<CEasyEnemy, CEnemy>( "CEasyEnemy" )
    .def( constructor<CXMLTreeNode&>() )
    .def( "init", &CEasyEnemy::Init )
    .def( "update", &CEasyEnemy::Update )
    .def( "render", &CEasyEnemy::Render )
  ];

  module( m_LS )
  [
    class_<CMapManager<CEnemy>>( "CMapManagerCEnemy" )
  ];
  module( m_LS )
  [
    class_<CEnemyManager, CMapManager<CEnemy>>( "CEnemyManager" )
    .def( constructor<>() )
    .def( "getEnemy", &CEnemyManager::GetResource )
    .def( "GetActualEnemy", &CEnemyManager::GetActualEnemy )
  ];
}