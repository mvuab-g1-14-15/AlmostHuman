#include "Shoot.h"

#include "EngineManagers.h"
#include "Utils\Defines.h"
#include "PhysicsManager.h"
#include "Utils\PhysicUserData.h"
#include "Actor\PhysicActor.h"
#include "Characters\Enemies\EnemyManager.h"
#include "Characters\Enemies\Enemy.h"
#include "Timer\Timer.h"

CShoot::CShoot()
	: CObject3D()
	, mSpeed( 0.0f )
	, mDamage( 0.0f )
	, mDirection( Math::Vect3f( 1.0f ) )
	, mImpacted( false )
{
}

CShoot::CShoot( float aSpeed, Math::Vect3f aDirection, Math::Vect3f aPosition, float aDamage )
	: CObject3D( aPosition, 0.0f, 0.0f )
	, mSpeed( aSpeed )
	, mDamage( aDamage )
	, mDirection( aDirection )
	, mImpacted( false )
{
	SetDirection( mDirection );
}

CShoot::~CShoot()
{
}

void CShoot::Init()
{
}

void CShoot::Update()
{
	if ( !mImpacted )
	{
		const Math::Vect3f& lDirection = GetDirection();
		const Math::Vect3f& lPosition = GetPosition();

		const Math::Vect3f lVelocity( lDirection * mSpeed * deltaTimeMacro );

		const float lLength( lVelocity.Length() );

		const Math::Vect3f lNewPosition = lPosition + lVelocity;

		SCollisionInfo hit_info;
		CPhysicUserData* lPUD = PhysXMInstance->RaycastClosestActor(lPosition, lDirection, 0xffffff, hit_info);

		if (lPUD)
		{
			const Math::Vect3f lCollisionPoint( hit_info.m_CollisionPoint );
			const float lDistance( lCollisionPoint.Distance( lNewPosition ) );
			if ( lDistance < lLength )
			{
				const std::string lName = lPUD->GetName();
				CEnemy* lEnemy = EnemyMInstance->GetResource(lName);

				if (lEnemy)
					lEnemy->AddDamage( mDamage );

				mImpacted = true;

				SetPosition( hit_info.m_CollisionPoint );
			}
			else
			{
				SetPosition( lNewPosition );
			}
		}
		else
		{
			SetPosition( lNewPosition );
		}

		MakeTransform();
	}
}

void CShoot::Render()
{
}

bool CShoot::Impacted()
{
	return mImpacted;
}