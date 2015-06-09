#include "PhysicUtils.h"
#include "Utils/Defines.h"
#include "Physics.h"
#include "Math\Matrix44.h"
#include "NxPhysics.h"

namespace PhysicUtils
{

template<typename T, typename U> bool Convert( const T& aFrom, U& aTo )
{
	ASSERT( false, "Invalid Conversion" );
	return false;
}

template<> bool Convert<Math::Mat44f,NxMat34>( const Math::Mat44f& aFrom, NxMat34& aTo )
{
	aTo.M = NxMat33
		( 
			NxVec3( aFrom.m00, aFrom.m10, aFrom.m20 ),
			NxVec3( aFrom.m01, aFrom.m11, aFrom.m21 ),
			NxVec3( aFrom.m02, aFrom.m21, aFrom.m22 )
		);
	aTo.t = NxVec3( aFrom.m03, aFrom.m13, aFrom.m23 );

	return true;
}

template<> bool Convert<Math::Vect3f,NxVec3>( const Math::Vect3f& aFrom, NxVec3& aTo )
{
	aTo = NxVec3( aFrom.x, aFrom.y, aFrom.z );
	return true;
}

}