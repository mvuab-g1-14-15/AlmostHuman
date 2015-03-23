#include "AIFunctions.h"
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
#include "StateMachine\StateMachine.h"
#include "StateMachine\State.h"
#include "Utils\MapManager.h"
#include "XML\XMLTreeNode.h"
#include "Object3D.h"
#include <vector>
#include "Math\Vector3.h"
#include "Pathfinding\AStar.h"

using namespace luabind;
using namespace Math;
template<class T>
void vector_add(std::vector<T>& vec, T val)
{
    vec.push_back(val);
}
template<class T>
size_t vector_getIdByResource(std::vector<T>& vec, T val)
{
    for(size_t i = 0 ; i < vec.size(); ++i)
       if( vec[i] == val)
            return i;
    return 0;
} 

template<class T>
T vector_get(std::vector<T>& vec, size_t i)
{
    return vec[i];
} 

void registerAI( lua_State* m_LS )
{
  module( m_LS )
  [
    class_<CCharacter, CObject3D>( "CCharacter" )
    .def( constructor<const std::string&>() )
    .def( "setTargetPosition", &CCharacter::SetTargetPosition )
    .def( "getTargetPosition", &CCharacter::GetTargetPosition )
    .def( "SetTargetPositionOriginal", &CCharacter::SetTargetPositionOriginal )
    .def( "init", ( void( CCharacter::* )( void ) )&CCharacter::Init )
    .def( "update", &CCharacter::Update )
    .def( "render", &CCharacter::Render )
    .def( "GetPosition", &CCharacter::GetPosition )
	.def( "AddDamage", &CCharacter::AddDamage )	
  ];
  module( m_LS )
  [
    class_<std::vector<std::string>>( "VectorStates")
    .def(constructor<std::vector<std::string>>())
    .def("getResource", &vector_get<std::string>)
    .def("getIdByResource", &vector_getIdByResource<std::string>)
    .def("size", &std::vector<std::string>::size)
  ];
  module( m_LS )
  [
    class_<CStateMachine, CName, CMapManager<CState>>( "CStateMachine" )
    .def( "GetStateName", &CStateMachine::GetStateName)
  ];
  module( m_LS )
  [
    class_<CEnemy, CCharacter>( "CEnemy" )
    .def( "ChangeState", &CEnemy::ChangeState )
    .def( "SetExit", &CEnemy::SetOnExit)
    .def( "getNameState", &CEnemy::GetNameStates)
    .def( "getCurrentState", &CEnemy::GetCurrentState)
    .def( "getStateMachine", &CEnemy::GetStateMachine)
  ];
  
  module( m_LS )
  [
    class_<std::vector<Vect3f>>( "VectorWaypoints")
    .def(constructor<std::vector<Vect3f>>())
    .def("GetCount", &std::vector<Vect3f>::size)
    .def("push_back",&vector_add<Vect3f>)
    .def("getResource", &vector_get<Vect3f>)
    .def("clear", &std::vector<Vect3f>::clear)
  ];
  module( m_LS )
  [
    class_<CPatrolEnemy, CEnemy>( "CPatrolEnemy" )
    .def( constructor<CXMLTreeNode&, CStateMachine*>() )
    .def( "init", &CPatrolEnemy::Init )
    .def( "update", &CPatrolEnemy::Update )
    .def( "render", &CPatrolEnemy::Render )
    .def( "GetWaypoints", &CPatrolEnemy::GetWaypoints)
    .def( "SetWaypoints", &CPatrolEnemy::SetWaypoints)
    .def( "GetCurrentPoint", &CPatrolEnemy::GetCurrentPoint)
    .def( "SetCurrentPoint", &CPatrolEnemy::SetCurrentPoint)
    .def( "getCount", &CPatrolEnemy::getCount)
  ];
  module( m_LS )
  [
    class_<CBossEnemy, CEnemy>( "CBossEnemy" )
    .def( constructor<CXMLTreeNode&, CStateMachine*>() )
    .def( "init", &CBossEnemy::Init )
    .def( "update", &CBossEnemy::Update )
    .def( "render", &CBossEnemy::Render )
  ];
  module( m_LS )
  [
    class_<CEasyEnemy, CEnemy>( "CEasyEnemy" )
    .def( constructor<CXMLTreeNode&, CStateMachine*>() )
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
  module( m_LS )
  [
    class_<CAStar>( "CAStar" )
    .def( constructor<>() )
    .def( "GetPath", &CAStar::GetPath )
  ];

}