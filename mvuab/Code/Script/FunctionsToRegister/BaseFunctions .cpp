#include "BaseFunctions.h"

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>

#include "Math\Matrix44.h"
#include "Math\Matrix34.h"
#include "Math\Vector3.h"
#include "Math\Vector4.h"
#include "Utils\Visible.h"
#include "Utils\Name.h"
#include "Timer\Timer.h"
#include "Logger\Logger.h"

#include "Math\MathTypes.h"
#include "Utils\Types.h"
#include "Math\Color.h"
#include "Utils\TemplatedVectorMapManager.h"

#include <sstream>

using namespace luabind;

void DA_Normalize( Math::Vect3f* vector3 )
{
  vector3->Normalize( Math::One<float32>() );
}

Math::Vect3f DA_MulMat44fVect3f( Math::Mat44f* mat, const Math::Vect3f& vec )
{
  return ( *mat ) * vec;
}

std::string Vect3f2String( Math::Vect3f* vector3 )
{
  std::ostringstream ss;
  ss << "(" << vector3->x << ", " << vector3->y << ", " << vector3->z << ")";
  std::string s( ss.str() );

  return s;
}

void registerBase( lua_State* m_LS )
{
  module( m_LS )
  [
    class_<Math::Vect3f>( "Vect3f" )
    .def( constructor<float, float, float>() )
    .def( constructor<const Math::Vect3f&>() )
    .def( constructor<float>() )
    .def( constructor<>() )

    .def( const_self + const_self )
    .def( const_self - const_self )
    .def( const_self * const_self )
    .def( const_self ^ const_self )
    .def( const_self == const_self )
    .def( const_self * float() )
    .def( const_self / float() )
    .def( float() * const_self )
    .def( float() / const_self )
    .def( const_self + float() )
    .def( const_self - float() )
    .def( float() + const_self )
    .def( float() - const_self )
    .def( - const_self )

    .def_readwrite( "x", &Math::Vect3f::x )
    .def_readwrite( "y", &Math::Vect3f::y )
    .def_readwrite( "z", &Math::Vect3f::z )

    .def( "CrossProduct", &Math::Vect3f::CrossProduct )
    .def( "Normalize", &DA_Normalize )
    .def("Length", &Math::Vect3f::Length)

    .def( "ToString", &Vect3f2String )
  ];
  module( m_LS )
  [
    class_<Math::Vect4f>( "Vect4f" )
    .def( constructor<float, float, float, float>() )
    .def( constructor<const Math::Vect4f&>() )
    .def( constructor<float>() )
    .def( constructor<>() )

    .def_readwrite( "x", &Math::Vect4f::x )
    .def_readwrite( "y", &Math::Vect4f::y )
    .def_readwrite( "z", &Math::Vect4f::z )
    .def_readwrite( "w", &Math::Vect4f::w )
  ];
  module( m_LS )
  [
    class_<Math::Mat44f>( "Mat44f" )
    .def( constructor<>() )
    .def( "Mul", DA_MulMat44fVect3f )

    .def( "SetRotByAngleX", &Math::Mat44f::SetRotByAngleX )
    .def( "SetRotByAngleY", &Math::Mat44f::SetRotByAngleY )
    .def( "SetRotByAngleZ", &Math::Mat44f::SetRotByAngleZ )
    //.def("SetScale", &Math::Mat44f::SetScale) //TODO to ask
    .def( "SetIdentity", &Math::Mat44f::SetIdentity )
    .def( "SetPos", ( Math::Mat44f & ( Math::Mat44f::* )( const Math::Vect3f& ) ) &Math::Mat44f::SetPos )
    .def( "GetPos", &Math::Mat44f::GetPos )
    .def( "GetScaleX", &Math::Mat44f::GetScaleX )
    .def( "GetScaleY", &Math::Mat44f::GetScaleY )
    .def( "GetScaleZ", &Math::Mat44f::GetScaleZ )
    .def( "GetAngleX", &Math::Mat44f::GetAngleX )
    .def( "GetAngleY", &Math::Mat44f::GetAngleY )
    .def( "GetAngleZ", &Math::Mat44f::GetAngleZ )
    .def( "GetRoll", &Math::Mat44f::GetRoll )
    .def( "GetPitch", &Math::Mat44f::GetPitch )
    .def( "GetYaw", &Math::Mat44f::GetYaw )
    .def( "GetPitchRollYaw", &Math::Mat44f::GetPitchRollYaw )
    .def( "GetRotedByAngleX", &Math::Mat44f::GetRotedByAngleX )
    .def( "GetRotedByAngleY", &Math::Mat44f::GetRotedByAngleY )
    .def( "GetRotedByAngleZ", &Math::Mat44f::GetRotedByAngleZ )
    .def( "GetRotedByAnglesXZ", &Math::Mat44f::GetRotedByAnglesXZ )
    .def( "GetRotedByAnglesYXZ", &Math::Mat44f::GetRotedByAnglesYXZ )
    .def( "GetTranslated", &Math::Mat44f::GetTranslated )
    .def( "GetScaled", &Math::Mat44f::GetScaled )
    .def( "RotByAngleX", &Math::Mat44f::RotByAngleX )
    .def( "RotByAngleY", &Math::Mat44f::RotByAngleY )
    .def( "RotByAngleZ", &Math::Mat44f::RotByAngleZ )
    .def( "RotByAnglesXZ", &Math::Mat44f::RotByAnglesXZ )
    .def( "RotByAnglesYXZ", &Math::Mat44f::RotByAnglesYXZ )
    .def( "GetScale", &Math::Mat44f::GetScale )
    .def( "GetScaleX", &Math::Mat44f::GetScaleX )
    .def( "GetScaleY", &Math::Mat44f::GetScaleY )
    .def( "GetScaleZ", &Math::Mat44f::GetScaleZ )
    .def( "Translate", &Math::Mat44f::Translate )
    .def( "Transpose", &Math::Mat44f::Transpose )
    .def( "RotByAngleX", &Math::Mat44f::RotByAngleX )
    .def( "RotByAngleY", &Math::Mat44f::RotByAngleY )
    .def( "RotByAngleZ", &Math::Mat44f::RotByAngleZ )
    .def( "RotByAnglesXZ", &Math::Mat44f::RotByAnglesXZ )
    .def( "RotByAnglesYXZ", &Math::Mat44f::RotByAnglesYXZ )
    .def( "GetAnglesYXZ", &Math::Mat44f::GetAnglesYXZ )
    .def( "GetAngleX", &Math::Mat44f::GetAngleX )
    .def( "GetAngleY", &Math::Mat44f::GetAngleY )
    .def( "GetAngleZ", &Math::Mat44f::GetAngleZ )
  ];
  module( m_LS )
  [
    class_<CName>( "CName" )
    .def( constructor<>() )
    .def( "SetName", &CName::SetName )
    .def( "GetName", &CName::GetName )
  ];
  module( m_LS )
  [
    class_<CVisible>( "CVisible" )
    .def( constructor<>() )
    .def( "SetVisible", &CVisible::SetVisible )
    .def( "GetVisible", &CVisible::GetVisible )
  ];
  module( m_LS )
  [
    class_<CTimer>( "CTimer" )
    .def( "GetElapsedTime", &CTimer::GetElapsedTime )
  ];
  module( m_LS )
  [
    class_<Math::CColor>( "CColor" )
    .def( constructor<float, float, float, float>() )
    .def( constructor<>() )
    .def( constructor<const Math::Vect4f&>() )
  ];

}