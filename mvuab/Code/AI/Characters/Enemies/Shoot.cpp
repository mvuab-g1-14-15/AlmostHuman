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
#include "Billboard\BillboardManager.h"
#include "Utils\IdManager.h"
#include "ScriptManager.h"
#include "Lights\OmniLight.h"
#include "Lights\LightManager.h"
#include "StaticMeshes\InstanceMesh.h"

#include "RenderableObject/Scene.h"
#include "RenderableObject\Room.h"

#include "RenderableObject/RenderableObjectsManager.h"
#include "Cameras\Camera.h"
#include "Cameras/CameraManager.h"

#include "Billboard/BillboardCore.h"
#include "Billboard/BillboardManager.h"
#include "Billboard/BillboardInstance.h"

CShoot::CShoot()
  : CObject3D()
  , mSpeed( 0.0f )
  , mDamage( 0.0f )
  , mDirection( Math::Vect3f( 1.0f ) )
  , mImpacted( false )
  , mShoot( 0 )
  , mShootGlow( 0 )
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

  const std::string lType( "ShootPlayer" );

  std::ostringstream ss;
  ss << lType << IdManager->GetId( lType );
  std::string lName( ss.str() );

  SetName( lName );

  mLight = new COmniLight();
  mLight->SetName( lName );
  mLight->SetIntensity( 0.65f );
  mLight->SetEndRangeAttenuation( 2.0f );
  mLight->SetColor( Math::colBLUE );
  mLight->SetPosition( aPosition );
  mLight->SetRenderShadows( false );

  mShoot = new CInstanceMesh(lName, "Blaster");
  SceneInstance->GetResource("core")->GetLayer("solid")->AddResource(lName, mShoot);
  mShootGlow = new CInstanceMesh(lName, "Blaster");
  SceneInstance->GetResource("core")->GetLayer("glow")->AddResource(lName, mShootGlow);

  mShoot->SetPosition(aPosition);
  mShootGlow->SetPosition(aPosition);

  CBillboardCore * lCore = BillboardMan->GetResource("blash");
  
  //mShoot->SetDirection( aDirection );
  //mShootGlow->SetDirection( aDirection );

  //mShoot->SetYaw( mShoot->GetYaw() - Math::half_pi32);
  //mShootGlow->SetYaw( mShootGlow->GetYaw() - Math::half_pi32);
  //mShoot->SetYaw( -Math::Utils::ATan2( aDirection.z, aDirection.x ) + Math::half_pi32 );
  //mShootGlow->SetYaw( -Math::Utils::ATan2( aDirection.z, aDirection.x ) + Math::half_pi32 );
  //float lPitch = Math::Utils::ATan2( aDirection.y,Math::Utils::Sqrt( aDirection.z * aDirection.z + aDirection.x * aDirection.x ) );
  //if (aDirection.z > 0)
  //    lPitch = -lPitch;
  //mShoot->SetPitch( lPitch );
  //mShootGlow->SetPitch( lPitch );

  CCamera* p_cam = CameraMInstance->GetCurrentCamera();
  float lYaw = p_cam->GetYaw();
  float lPitch = p_cam->GetPitch();
  Math::Vect3f lSide = p_cam->GetVecUp().CrossProduct( p_cam->GetDirection() ).Normalize();
  mInstance = new CBillboardInstance();
  mInstance->ChangePosition( aPosition + lSide * 0.23f );
  //mInstance->ChangeVisibility( true );
  //lCore->AddInstance(mInstance);

  mShoot->SetYaw( -lYaw );
  mShoot->SetPitch(lPitch);
  mShootGlow->SetYaw( -lYaw );
  mShootGlow->SetPitch(lPitch);

  mShoot->MakeTransform();
  mShootGlow->MakeTransform();

  if ( !LightMInstance->AddResource( lName, mLight ) )
  {
    CHECKED_DELETE( mLight );
    //mLight = LightMInstance->GetConstResource( lName );
  }
}

CShoot::~CShoot()
{
  LightMInstance->RemoveResource( GetName() );
  //mInstance->ChangeVisibility(false);

  SceneInstance->GetResource("core")->GetLayer("solid")->RemoveResource(GetName());
  SceneInstance->GetResource("core")->GetLayer("glow")->RemoveResource(GetName());
}

bool CShoot::Init()
{
  return true;
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
		  {
            ScriptMInstance->RunCode( "enemy_manager:AddDamage('" + lName + "')" );
			ScriptMInstance->RunCode( "local enemy = enemy_manager:GetEnemy('" + lName + "');enemy.Suspected = true; enemy.SuspectedPosition = g_Player:GetPosition();enemy:SetIsHurting();" );
		  }

          //lEnemy->AddDamage( mDamage );
        }

        mImpacted = true;

        SetPosition( hit_info.m_CollisionPoint );
      }
      else
        SetPosition( lNewPosition );
    }
    else
      SetPosition( lNewPosition );

    mLight->SetPosition( GetPosition() );
    mLight->MakeTransform();

    MakeTransform();
    
    mShoot->SetPosition(GetPosition());
    mShootGlow->SetPosition(GetPosition());

    //mShoot->SetDirection( lDirection );
    //mShootGlow->SetDirection( lDirection );

    //mShoot->SetYaw( mShoot->GetYaw() - Math::half_pi32);
    //mShootGlow->SetYaw( mShootGlow->GetYaw() - Math::half_pi32);

    mShoot->MakeTransform();
    mShootGlow->MakeTransform();
  }
}

void CShoot::Render()
{

}

bool CShoot::Impacted()
{
  return mImpacted;
}