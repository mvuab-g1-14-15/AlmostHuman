#include "Character.h"
#include "Actor\PhysicController.h"
#include "Utils\PhysicUserData.h"
#include "PhysicsManager.h"
#include "Math\LerpAnimator3D.h"
#include "Timer\Timer.h"
#include "Core.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"


CCharacter::CCharacter( const std::string& Name )
  : CName( Name ), CObject3D()
  , m_Speed( 0.06f )
  , m_Life( 20.0f )
{
}

void CCharacter::ExecuteAI()
{
}

void CCharacter::Update()
{
  CPhysicUserData* l_PUD = CPhysicsManager::GetSingletonPtr()->GetUserData( "CharacterController" );
  CPhysicController* l_CharacterController = l_PUD->GetController();
  Math::Vect3f l_Distance = l_CharacterController->GetPosition() - m_Controller->GetPosition();
  float l_Cantidad = ( Math::Utils::Pow2( l_Distance ) ).x;
  l_Cantidad = Math::Utils::Sqrt( l_Cantidad );

  if ( l_Cantidad < 1.0f )
    SetTargetPosition( l_CharacterController->GetPosition() );
  else
    SetTargetPosition( m_TargetPositionOriginal );

  Math::Vect3f l_Position = m_Controller->GetPosition();
  float l_Yaw = m_Controller->GetYaw();
  Math::Vect3f l_Direction( Math::Utils::Cos( l_Yaw ) , 0.0f, Math::Utils::Sin( l_Yaw ) );
  l_Direction = l_Direction.GetNormalized();
  Math::Vect3f l_DessiredDirection = m_TargetPosition - l_Position;
  l_DessiredDirection.y = 0;

  if ( l_DessiredDirection != Math::Vect3f( 0, 0, 0 ) )
    l_DessiredDirection = l_DessiredDirection.GetNormalized();

  Math::CLerpAnimator3D l_Interpolator3D;
  l_Interpolator3D.SetValues( l_Direction, l_DessiredDirection, 1.0f, Math::FUNC_CONSTANT );
  Math::Vect3f l_LookAt;
  l_Interpolator3D.Update( 1, l_LookAt );
  m_Controller->SetYaw( Math::Utils::ATan2( l_LookAt.z, l_LookAt.x ) );
  SetYaw( Math::Utils::ATan2( l_LookAt.z, l_LookAt.x ) );

  if ( l_LookAt != Math::Vect3f( 0, 0, 0 ) )
    l_LookAt = l_LookAt.GetNormalized();

  float n = l_LookAt.Length();
  m_Controller->Move( l_LookAt * m_Speed, deltaTime );
  SetPosition( m_Controller->GetPosition() );
  /* CRenderableObject* l_Box =
     CCore::GetSingletonPtr()->GetRenderableObjectsLayersManager()->GetResource( "solid" )->GetResource( "Pyramid001" );
   l_Box->SetPosition( m_Controller->GetPosition() + Vect3f( 0, 2, 0 ) );
   l_Box->SetYaw( m_Controller->GetYaw() );*/
}

void CCharacter::Render()
{
  /* CGraphicsManager* l_GM = CCore::GetSingletonPtr()->GetGraphicsManager();
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
  m_Height = 1.0f;
  m_Radius = 0.2f;
  CPhysicsManager* l_PM = CPhysicsManager::GetSingletonPtr();
  l_PM->AddController( m_Name, m_Radius, m_Height, 2.0f, 2.0f, 2.0f, Math::Vect3f( -10, 0.0, 10 ), GetCollisionGroup(), -10.0 );
  m_Controller = l_PM->CMapManager<CPhysicController>::GetResource( m_Name );
  SetPosition( m_Controller->GetPosition() );
  SetTargetPosition( m_Controller->GetPosition() );
  SetTargetPositionOriginal( m_Controller->GetPosition() );
}
void CCharacter::Init( CXMLTreeNode& Node )
{
  m_AIPath = Node.GetPszProperty( "lua_path", "no_path" );
  CPhysicsManager* l_PM = CPhysicsManager::GetSingletonPtr();
  l_PM->AddController( m_Name, Node.GetFloatProperty( "radius", 0.2f ),
                       Node.GetFloatProperty( "height", 1.0f ),
                       Node.GetFloatProperty( "slope", 0.2f ),
                       Node.GetFloatProperty( "skin_width", 0.01f ),
                       Node.GetFloatProperty( "step", 0.5f ),
                       Node.GetVect3fProperty( "pos", Math::Vect3f( 0, 4.0, 0 ) ),
                       GetCollisionGroup(), Node.GetFloatProperty( "gravity", -10.0f ) );
  m_Controller = l_PM->CMapManager<CPhysicController>::GetResource( m_Name );
  SetPosition( m_Controller->GetPosition() );
  SetTargetPosition( m_Controller->GetPosition() );
  SetTargetPositionOriginal( m_Controller->GetPosition() );
}

ECollisionGroup CCharacter::GetCollisionGroup()
{
	return ECG_ENEMY;
}