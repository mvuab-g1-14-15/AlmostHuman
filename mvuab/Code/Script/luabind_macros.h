#ifndef __LUA_MACROS_H__
#define __LUA_MACROS_H__

#define LUA_BEGIN_DECLARATION( LS ) luabind::module( LS ) [
#define LUA_END_DECLARATION ];

#define LUA_DECLARE_CLASS( CL )                       luabind::class_< CL >( #CL )
#define LUA_DECLARE_DERIVED_CLASS( CL, BASE )         luabind::class_< CL, BASE >( #CL )
#define LUA_DECLARE_DERIVED_CLASS2( CL, BASE, BASE1 ) luabind::class_< CL, bases< BASE, BASE1 > >( #CL )

#define LUA_DECLARE_DEFAULT_CTOR                       .def(luabind::constructor<>())
#define LUA_DECLARE_CTOR_1( P1 )                       .def(luabind::constructor<P1>())
#define LUA_DECLARE_CTOR_2( P1, P2 )                   .def(luabind::constructor<P1,P2>())
#define LUA_DECLARE_CTOR_3( P1, P2, P3 )               .def(luabind::constructor<P1,P2,P3>())
#define LUA_DECLARE_CTOR_4( P1, P2, P3, P4 )           .def(luabind::constructor<P1,P2,P3,P4>())
#define LUA_DECLARE_CTOR_5( P1, P2, P3, P4, P5 )       .def(luabind::constructor<P1,P2,P3,P4,P5>())
#define LUA_DECLARE_CTOR_6( P1, P2, P3, P4, P5, P6 )   .def(luabind::constructor<P1,P2,P3,P4,P5,P6>())

#define LUA_DECLARE_METHOD( CL, NAME )                .def( #NAME, &CL::NAME )
#define LUA_DECLARE_METHOD_PROTO( CL, NAME, PROTO )   .def( #NAME, (PROTO) &CL::NAME )

#define LUA_DECLARE_METHOD_WITHOUT_CLASS( NAME )      def( #NAME, &NAME )
#define LUA_DECLARE_METHOD_WITHOUT_CLASS1( NAME )      def( #NAME, NAME )

#define LUA_BEGIN_ENUM( NAME )                        .enum_( #NAME )[
#define LUA_ENUM_VALUE( NAME, VAL )                   luabind::value( #NAME, VAL )
#define LUA_END_ENUM( )                               ]

#define LUA_BEGIN_SCOPE                               .scope [
#define LUA_DECLARE_METHOD_SCOPE( CL, NAME )          def( #NAME, &CL::NAME )
#define LUA_END_SCOPE                                 ]
#ifdef _HAS_LUA_BIND_

#include <lua.hpp>

#include <luabind/luabind.hpp>

// ## is a literal, # is a string

#define LUA_CREATE_MODULE( L ) luabind::module_ module( L )

#define LUA_CREATE_MODULE_NS( L, NAMESPACE ) luabind::module_( L, #NAMESPACE )

#define LUA_CLASS( CL ) __lua##CL

#define LUA_BEGIN_SCOPE( CL )   __lua##CL.scope[
#define LUA_END_SCOPE()         ]

#define LUA_BEGIN_ENUM( CL, NAME )    __lua##CL.enum_( #NAME )[
#define LUA_ENUM_VALUE( NAME, VAL ) luabind::value( #NAME, VAL )
#define LUA_END_ENUM( )         ]

// regular classes
#define LUA_DECLARE_CLASS( CL )                       luabind::class_< CL > __lua##CL( #CL)
#define LUA_DECLARE_CLASS_SCOPED( CL )                luabind::class_< CL, boost::shared_ptr<CL> > __lua##CL( #CL)

// shared ptrs
#define LUA_DECLARE_DERIVED_CLASS( CL, BASE )         luabind::class_< CL, BASE > __lua##CL( #CL)
#define LUA_DECLARE_DERIVED_CLASS_SCOPED( CL, BASE )  luabind::class_< CL, BASE, boost::shared_ptr<BASE> > __lua##CL( #CL)

// inner classes
#define LUA_DECLARE_INNER_CLASS( SCOPE, CL )                       luabind::class_< SCOPE::CL > __lua##CL( #CL)
#define LUA_DECLARE_INNER_DERIVED_CLASS( SCOPE, CL, BASE )         luabind::class_< SCOPE::CL, BASE > __lua##CL( #CL)
#define LUA_DECLARE_INNER_DERIVED_CLASS_SCOPED( SCOPE, CL, BASE )  luabind::class_< SCOPE::CL, BASE, boost::shared_ptr<BASE> > __lua##CL( #CL )
#define LUA_END_DECLARE()

#define LUA_DECLARE_CTOR( CL )              __lua##CL.def(luabind::constructor<>())
#define LUA_DECLARE_CTOR_1( CL, P1 )        __lua##CL.def(luabind::constructor<P1>())
#define LUA_DECLARE_CTOR_2( CL, P1, P2 )    __lua##CL.def(luabind::constructor<P1,P2>())
#define LUA_DECLARE_CTOR_3( CL, P1, P2, P3 )    __lua##CL.def(luabind::constructor<P1,P2,P3>())
#define LUA_DECLARE_CTOR_4( CL, P1, P2, P3, P4 )    __lua##CL.def(luabind::constructor<P1,P2,P3,P4>())
#define LUA_DECLARE_CTOR_5( CL, P1, P2, P3, P4, P5 )    __lua##CL.def(luabind::constructor<P1,P2,P3,P4,P5>())
#define LUA_DECLARE_CTOR_6( CL, P1, P2, P3, P4, P5, P6 )    __lua##CL.def(luabind::constructor<P1,P2,P3,P4,P5,P6>())

#define LUA_DECLARE_METHOD( CL, NAME )              __lua##CL.def( #NAME, &CL::NAME )
#define LUA_DECLARE_METHOD_PROTO( CL, NAME, PROTO )  __lua##CL.def( #NAME, PROTO )

#define LUA_DEFINE_CLASS( M, CL )              M[ __lua##CL ]

#endif // _HAS_LUA_BIND_

#endif /* LUA_MACROS_H_ */
