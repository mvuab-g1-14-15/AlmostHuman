#include "ScriptManager\FunctionsToRegister\AIFunctions.h"

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "Characters/Character.h"
#include "Characters\Enemies\Enemy.h"
#include "Characters\Enemies\EnemyManager.h"
#include "XML\XMLTreeNode.h"


using namespace luabind;

void registerAI( lua_State* m_LS )
{
  module( m_LS )
  [
    class_<CCharacter>( "CCharacter" )
    .def( constructor<const std::string&>() )
    .def( "SetTargetPosition", &CCharacter::SetTargetPosition )
    .def( "Init", ( void( CCharacter::* )( void ) )&CCharacter::Init )
    .def( "Update", &CCharacter::Update )
    .def( "Render", &CCharacter::Render )
  ];

  module( m_LS )
  [
    class_<CEnemy>( "CEnemy" )
    .def( constructor<CXMLTreeNode&>() )
    .def( "update", &CEnemy::Update )
    .def( "render", &CEnemy::Render )
  ];
  module( m_LS )
  [
    class_<CEnemyManager>( "CEnemyManager" )
    .def( constructor<>() )
    .def( "getEnemy", &CEnemyManager::GetResource )
  ];
}