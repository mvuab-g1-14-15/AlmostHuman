#include "Reports\PhysicMath::collisionReport.h"

void  CPhysicMath::collisionReport::onContactNotify(NxContactPair& _Pair, NxU32 _Events)
{
	if ( _Pair.isDeletedActor [0] || _Pair.isDeletedActor [1] )
		return;

	NxActor* l_Actor = _Pair.actors[0];
	CPhysicUserData* l_EntityMath::collision1 = ( CPhysicUserData* ) l_Actor->userData;  

	l_Actor = _Pair.actors[1];
	CPhysicUserData* l_EntityMath::collision2	= ( CPhysicUserData* ) l_Actor->userData;

	if ( _Events & NX_NOTIFY_ON_START_TOUCH)           
	{                           
		OnStartTouch ( l_EntityMath::collision1, l_EntityMath::collision2 );
	}            
  if ( _Events & NX_NOTIFY_ON_END_TOUCH)            
	{                
		OnEndTouch ( l_EntityMath::collision1, l_EntityMath::collision2 );         
	}
  if ( _Events & NX_NOTIFY_ON_TOUCH)            
	{                
		OnTouch ( l_EntityMath::collision1, l_EntityMath::collision2 );         
	}
  if ( _Events & NX_NOTIFY_ON_START_TOUCH_FORCE_THRESHOLD)           
	{                           
		OnStartTouchForceThreshold( l_EntityMath::collision1, l_EntityMath::collision2 );
	}            
  if ( _Events & NX_NOTIFY_ON_END_TOUCH_FORCE_THRESHOLD)            
	{                
		OnEndTouchForceThreshold( l_EntityMath::collision1, l_EntityMath::collision2 );         
	}
  if ( _Events & NX_NOTIFY_ON_TOUCH_FORCE_THRESHOLD)            
	{                
		OnTouchForceThreshold( l_EntityMath::collision1, l_EntityMath::collision2 );         
	}
}