#include "AIFunctions.h"
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "StateMachine\StateMachine.h"
#include "StateMachine\State.h"
#include "Utils\MapManager.h"
#include "XML\XMLTreeNode.h"
#include "Object3D.h"
#include <vector>
#include "Math\Vector3.h"
#include "Pathfinding\AStar.h"
#include "RenderableObject\RenderableObject.h"

#include "luabind_macros.h"

using namespace luabind;
using namespace Math;
template<class T>
void vector_add( std::vector<T>& vec, T val )
{
    vec.push_back( val );
}
template<class T>
size_t vector_getIdByResource( std::vector<T>& vec, T val )
{
    for ( size_t i = 0 ; i < vec.size(); ++i )
        if ( vec[i] == val )
            return i;

    return 0;
}

template<class T>
T vector_get( std::vector<T>& vec, size_t i )
{
    return vec[i];
}

void registerAI( lua_State* aLuaState )
{
    module( aLuaState )
    [
        class_<std::vector<Vect3f>>( "VectorWaypoints" )
        .def( constructor<std::vector<Vect3f>>() )
        .def( "GetResource", &vector_get<Vect3f> )
    ];

}