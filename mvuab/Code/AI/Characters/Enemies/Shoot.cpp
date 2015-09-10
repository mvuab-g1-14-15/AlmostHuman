#include "Shoot.h"

#include <sstream>

#include "EngineManagers.h"
#include "Utils\Defines.h"
#include "PhysicsManager.h"
#include "Utils\PhysicUserData.h"
#include "Actor\PhysicActor.h"
#include "Characters\Enemies\EnemyManager.h"
#include "Characters\Enemies\Enemy.h"
#include "Timer\Timer.h"
#include "Billboard\Billboard.h"
#include "Billboard\BillboardManager.h"
#include "Utils\IdManager.h"
#include "ScriptManager.h"
#include "Lights\OmniLight.h"
#include "Lights\LightManager.h"

CShoot::CShoot()
  : CObject3D()
  , mSpeed( 0.0f )
  , mDamage( 0.0f )
  , mDirection( Math::Vect3f( 1.0f ) )
  , mImpacted( false )
  , mBillboard( new CBillboard() )
{
}

CShoot::CShoot( float aSpeed, Math::Vect3f aDirection, Math::Vect3f aPosition, float aDamage )
  : CObject3D( aPosition, 0.0f, 0.0f )
  , mSpeed( aSpeed )
  , mDamage( aDamage )
  , mDirection( aDirection )
  , mImpacted( false )
  , mBillboard( 0 )
{
  SetDirection( mDirection );

  const std::string lType( "ShootPlayer" );

  std::ostringstream ss;
  ss << lType << IdManager->GetId( lType );
  std::string lName( ss.str() );

  SetName( lName );
  mBillboard = new CBillboard();

  if ( !BillboardMan->AddResource( lName, mBillboard ) )
  {
    CHECKED_DELETE( mBillboard );
    mBillboard = BillboardMan->GetConstResource( lName );
  }

  mLight = new COmniLight();
  mLight->SetName( lName );
  mLight->SetIntensity( 0.65f );
  mLight->SetEndRangeAttenuation( 2.0f );
  mLight->SetColor( Math::colRED );
  mLight->SetPosition( aPosition );
  mLight->SetRenderShadows( false );

  if ( !LightMInstance->AddResource( lName, mLight ) )
  {
    CHECKED_DELETE( mLight );
    //mLight = LightMInstance->GetConstResource( lName );
  }
}

CShoot::~CShoot()
{
  // TODO: Remove the billboard in the manager
  mBillboard->SetActive( false );
  LightMInstance->RemoveResource( GetName() );
}

bool CShoot::Init()
{
  bool lOk( true );
  lOk = lOk && mBillboard->Init( "ShootBillBoard", GetPosition(), 1.0f, 0.0f, 1.0f, "Data/textures/particles/fire3.png", "BillboardAlignedToCameraTechnique" );
  ASSERT( lOk, "Error initing the shoot" );

  if( !BillboardMan->Exist( GetName() ) )
	BillboardMan->AddResource( GetName(), mBillboard );

  return lOk;
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
    CPhysicUserData* lPUD = PhysXMInstance->RaycastClosestActor( lPosition, lDirection, 0xffffff, hit_info );

    if ( lPUD )
    {
      const Math::Vect3f lCollisionPoint( hit_info.m_CollisionPoint );
      const float lDistance( lCollisionPoint.Distance( lNewPosition ) );

      if ( lDistance < lLength )
      {
        const std::string lName = lPUD->GetName();

        if ( lName == "Player" )
          ScriptMInstance->RunCode( "g_Player:AddDamage(5.0)" );
        else
        {

          //CEnemy* lEnemy = EnemyMInstance->GetResource( lName );

          if ( lPUD->GetController() )
            ScriptMInstance->RunCode( "g_EnemyManager:AddDamage('" + lName + "')" );

          //lEnemy->AddDamage( mDamage );
        }

        mImpacted = true;

        SetPosition( hit_info.m_CollisionPoint );

        mBillboard->SetActive( false );
      }
      else
        SetPosition( lNewPosition );
    }
    else
      SetPosition( lNewPosition );

    mBillboard->SetPosition( GetPosition() );
    mBillboard->MakeTransform();

    mLight->SetPosition( GetPosition() );
    mLight->MakeTransform();

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