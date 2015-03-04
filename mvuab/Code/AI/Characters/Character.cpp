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
	: CName( Name )
	, m_Speed( 0.02f )
	, m_Life( 0.0f )
{
}

CCharacter::~CCharacter()
{
  CPhysicUserData* l_PUD = m_PController->GetUserData();
  CHECKED_DELETE( l_PUD );
  CHECKED_DELETE( m_PController );
}

void CCharacter::ExecuteAI()
{
}

void CCharacter::Update()
{
  Math::Vect3f l_Position = m_PController->GetPosition();
  float l_Yaw = m_PController->GetYaw();
  Math::Vect3f l_Direction( Math::Utils::Cos( l_Yaw ) , 0.0f, Math::Utils::Sin( l_Yaw ) );
  l_Direction = l_Direction.GetNormalized();
  Math::Vect3f l_DessiredDirection = ( m_TargetPosition - l_Position ).GetNormalized();
  Math::CLerpAnimator3D l_Interpolator3D;
  l_Interpolator3D.SetValues( l_Direction, l_DessiredDirection, 1.0f, Math::FUNC_CONSTANT );
  Math::Vect3f l_LookAt;
  l_Interpolator3D.Update( 1, l_LookAt );
  m_PController->SetYaw( Math::Utils::ATan2( l_LookAt.z, l_LookAt.x ) );
  m_PController->Move( l_LookAt.GetNormalized() * m_Speed, deltaTime );
  CRenderableObject* l_Box =
    CCore::GetSingletonPtr()->GetRenderableObjectsLayersManager()->GetResource( "solid" )->GetResource( "Pyramid001" );
  l_Box->SetPosition( m_PController->GetPosition() + Vect3f( 0, 2, 0 ) );
  l_Box->SetYaw( m_PController->GetYaw() );
}

void CCharacter::Render()
{
}

void CCharacter::Init()
{
  m_PController = new CPhysicController( 2.0,
                                         2.0,
                                         2.0,
                                         2.0,
                                         2.0,
                                         GetCollisionGroup(),
                                         GetPhysicsUserData(),
                                         Math::Vect3f( -10, 4.0, -10 ),
                                         -10.0 );
  CPhysicsManager* l_PM = CPhysicsManager::GetSingletonPtr();
  l_PM->AddPhysicController( m_PController );
  m_TargetPosition = Math::Vect3f( 10, 0.0f, 10 );
}
void CCharacter::Init( CXMLTreeNode& Node )
{
  m_AIPath = Node.GetPszProperty( "lua_path", "no_path" );
  m_PController = new CPhysicController( Node.GetFloatProperty( "radius", 2.0 ),
                                         Node.GetFloatProperty( "height", 2.0 ),
                                         Node.GetFloatProperty( "slope", 2.0 ),
                                         Node.GetFloatProperty( "skin_width", 2.0 ),
                                         Node.GetFloatProperty( "step", 2.0 ),
                                         GetCollisionGroup(), GetPhysicsUserData(),
                                         Node.GetVect3fProperty( "initial_position", Math::Vect3f( 0, 4.0, 0 ) ),
                                         Node.GetFloatProperty( "gravity", 10.0 ) );
  CPhysicsManager* l_PM = CPhysicsManager::GetSingletonPtr();
  l_PM->AddPhysicController( m_PController );
  m_TargetPosition = Math::Vect3f( 10, 0.0f, 10 );
}

ECollisionGroup CCharacter::GetCollisionGroup()
{
  return ECG_PLAYER;
}

CPhysicUserData* CCharacter::GetPhysicsUserData()
{
  std::string& l_UserDataName = m_Name + std::string( "UserData" );
  CPhysicUserData* l_PUD = new CPhysicUserData( l_UserDataName );
  // TODO CHECK THIS
  l_PUD->SetPaint( true );
  l_PUD->SetColor( colWHITE );
  // TODO CHECK THIS
  return l_PUD;
}