#include "Character.h"
#include "Actor\PhysicController.h"
#include "Utils\PhysicUserData.h"
#include "PhysicsManager.h"
#include "Math\LerpAnimator3D.h"
#include "Timer\Timer.h"

#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "EngineManagers.h"

#include "Utils\Defines.h"

CCharacter::CCharacter( const std::string& Name )
  : CName( Name ), CObject3D()
  , m_Speed( 0.06f )
  , m_Life( 20.0f )
  , m_TimeToShoot( 0.0f )
  , m_MaxTimeToShoot( 2.0f )
{
}

void CCharacter::ExecuteAI()
{
}

void CCharacter::Update()
{
}

void CCharacter::Render()
{
  /*  CGraphicsManager* l_GM = GraphicsInstance;
      Mat44f m;
      m.Translate( m_Position );
      m.RotByAnglesYXZ( GetYaw(), GetPitch(), GetRoll() );
      l_GM->SetTransform( m );
      l_GM->DrawCapsule(m_Radius, m_Height);
      l_GM->DrawAxis( 1 );
      m.SetIdentity();
      l_GM->SetTransform( m );
  */
}

void CCharacter::Init()
{
  /*m_Height = 2.0f;
  m_Radius = 0.2f;
  CPhysicsManager* l_PM = PhysXMInstance;
  l_PM->AddController( m_Name, m_Radius, m_Height / 2.0f, 2.0f, 2.0f, 2.0f, Math::Vect3f( -10, 0.0, 10 ),
                       GetCollisionGroup(),
                       -10.0 );
  m_Controller = l_PM->CMapManager<CPhysicController>::GetResource( m_Name );
  SetPosition( m_Controller->GetPosition() );
  SetTargetPosition( m_Controller->GetPosition() );
  SetTargetPositionOriginal( m_Controller->GetPosition() );*/
}

void CCharacter::Init( CXMLTreeNode& Node )
{
  m_AIPath = Node.GetAttribute<std::string>( "lua_path", "no_path" );
  CPhysicsManager* l_PM = PhysXMInstance;
  l_PM->AddController( m_Name, Node.GetAttribute<float>( "radius", 0.4f ),
                       Node.GetAttribute<float>( "height", 2.0f ),
                       Node.GetAttribute<float>( "slope", 0.2f ),
                       Node.GetAttribute<float>( "skin_width", 0.01f ),
                       Node.GetAttribute<float>( "step", 0.5f ),
                       Node.GetAttribute<Math::Vect3f>( "pos", Math::Vect3f( 0, 4.0, 0 ) ),
                       GetCollisionGroup(), Node.GetAttribute<float>( "gravity", -10.0f ) );
  m_Controller = l_PM->CMapManager<CPhysicController>::GetResource( m_Name );
  SetPosition( m_Controller->GetPosition() );
  SetTargetPosition( m_Controller->GetPosition() );
  SetTargetPositionOriginal( m_Controller->GetPosition() );
}

ECollisionGroup CCharacter::GetCollisionGroup()
{
  return ECG_ENEMY;
}

void CCharacter::Move( Math::Vect3f direction, float dt )
{
  float l_DirYaw = Math::Utils::ATan2( direction.z, direction.x );
  float l_Yaw = m_Controller->GetYaw();

  float l_YawDif = l_DirYaw - l_Yaw;

  float l_PrevYaw = l_Yaw;

  if ( Math::Utils::Abs( l_YawDif ) < 0.1f )
    m_Controller->Move( direction * 2.0f, dt );
  else
  {
    m_Controller->Move( Math::Vect3f( 0.0f ), dt );
    l_Yaw += ( l_YawDif > 0 ? 1 : -1 ) * 1.0f * deltaTimeMacro;

    if ( ( l_Yaw < 0.0f && l_PrevYaw > 0.0f ) || ( l_Yaw > 0.0f && l_PrevYaw < 0.0f ) )
      l_Yaw = l_DirYaw;
  }

  m_Controller->SetYaw( l_Yaw );
  SetYaw( -l_Yaw + Math::half_pi32 );
}

float CCharacter::GetHeight()
{
  return m_Controller->GetHeight();
}

float CCharacter::GetRadius()
{
  return m_Controller->GetRadius();
}