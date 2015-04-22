#define __DONT_INCLUDE_MEM_LEAKS__
#include "Joints\PhysicFixedJoint.h"
#include "NxFixedJointDesc.h"
#include "Actor\PhysicActor.h"
////#include "Utils\Base.h"

#include "Utils\Defines.h"
#include "Logger\Logger.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CPhysicFixedJoint::CPhysicFixedJoint()
  : m_pJoint( NULL )
  , m_pFixedDesc( NULL )
{
  m_pFixedDesc = new NxFixedJointDesc;
}

CPhysicFixedJoint::~CPhysicFixedJoint()
{
  CHECKED_DELETE( m_pFixedDesc );
}

void CPhysicFixedJoint::CreateJoint( NxJoint* joint )
{
  assert( joint );
  m_pJoint = joint;
  CHECKED_DELETE( m_pFixedDesc );
}

void CPhysicFixedJoint::SetInfo( CPhysicActor* actorA,  CPhysicActor* actorB )
{
  if ( actorA == NULL )
  {
    LOG_ERROR_APPLICATION( "CPhysicFixedJoint:: El primer actor pasado como argumento no puede ser null" );
    return;
  }

  m_pFixedDesc->actor[0] = actorA->GetPhXActor();

  if ( actorB != NULL )
    m_pFixedDesc->actor[1] = actorB->GetPhXActor();
  else
    m_pFixedDesc->actor[1] = NULL;
}