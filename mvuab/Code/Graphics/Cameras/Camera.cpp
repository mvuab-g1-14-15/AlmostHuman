#include "Camera.h"
#include "Math\MathUtils.h"
#include "Object3D.h"
#include "Utils\Defines.h"
#include "ActionManager.h"

CCamera::CCamera( Vect3f aEyePosition, Vect3f aLookAt, float32 aZNear, float32 aZFar, float32 aFOV, float32 aAspectRatio, std::string aType ) 
	: m_FOV(aFOV),
	m_AspectRatio(aAspectRatio),
	m_ZNear(aZNear),
	m_ZFar(aZFar),
	m_EyePosition( aEyePosition ),
	m_LookAt( aLookAt ),
	m_pAttachedObject( 0 ),
    m_TypeStr(aType),
    m_Speed( 0.1f )
{
	InitYawAndPitch();
}

CCamera::CCamera( Vect3f aEyePosition, Vect3f aLookAt, CObject3D* apAttachedObject, std::string aType )
	: m_FOV(mathUtils::Deg2Rad(60.f)),
	m_AspectRatio(4.f/3.f),
	m_ZNear(0.1f),
	m_ZFar(100.f),
	m_EyePosition( aEyePosition ),
	m_LookAt( aLookAt ),
	m_pAttachedObject( apAttachedObject ),
    m_TypeStr(aType),
    m_Speed( 0.1f )
{
	InitYawAndPitch();
}

CCamera::~CCamera()
{
	CHECKED_DELETE(m_pAttachedObject);
}

Vect3f CCamera::GetDirection() const
{
	return (m_LookAt - m_EyePosition).Normalize();
}

Vect3f CCamera::GetVecUp() const
{
	Vect3f worldUpVector(0.0f,1.0f,0.0f);
	Vect3f directionVector = GetDirection();
	Vect3f eyeSideVector = worldUpVector.CrossProduct(directionVector);
	return (directionVector.CrossProduct(eyeSideVector)).Normalize();
}

Vect3f CCamera::GetVecSide() const
{
	Vect3f direction = GetDirection();
	Vect3f up = GetVecUp();

	return up.CrossProduct( direction ).Normalize();
}

void CCamera::AddYawPitch( float32 dx, float32 dy )
{
	m_Yaw -= ( dx * 0.01f);
	m_Pitch += ( dy* 0.01f );

	m_Pitch = mathUtils::Clamp(m_Pitch, PITCH_LIM_UP, PITCH_LIM_DOWN );

	if(m_Yaw > ePIf )
        m_Yaw = -ePIf + (m_Yaw - ePIf);
    else if(m_Yaw < -ePIf)
        m_Yaw = ePIf - (-ePIf + m_Yaw);

	RecalculateCameraData();
}

void CCamera::InitYawAndPitch()
{
	float32 distanceEyeLookAt = m_EyePosition.Distance(m_LookAt);
	m_Yaw = mathUtils::ACos( m_EyePosition.x / distanceEyeLookAt );
	m_Pitch= mathUtils::ASin( m_EyePosition.y / distanceEyeLookAt );
}

void CCamera::Move( CameraMovementDirecction aMovementDir, float32 speed )
{
	Vect3f VectorSide = GetVecSide();
	Vect3f VectorUp = GetVecUp();

	switch( aMovementDir )
	{
	case UP:
			m_EyePosition += VectorUp * speed;
			m_LookAt += VectorUp * speed;
		break;
	case DOWN:
		{
			m_EyePosition -= VectorUp * speed;
			m_LookAt -= VectorUp * speed;
		}
		break;
	case FORWARD:
		{
			m_EyePosition += GetDirection() * speed;
		} 
		break;
	case BACKWARD:
		{
			m_EyePosition -= GetDirection() * speed;
		} 
		break;
	case LEFT:
		{
			m_EyePosition -= VectorSide * speed;
			m_LookAt -= VectorSide * speed;
		} 
		break;
	case RIGHT:
		{
			m_EyePosition += VectorSide * speed;
			m_LookAt += VectorSide * speed;
		} 
		break;
	}

	m_pAttachedObject->SetPosition(m_LookAt);
}

void CCamera::Update(float32 deltaTime)
{
    CActionManager* pActionManager = CActionManager::GetSingletonPtr();
    if( pActionManager->DoAction("MoveForward") )
    {
        Move( FORWARD, m_Speed );
    }
    if( pActionManager->DoAction("MoveBackward") )
    {
        Move( BACKWARD, m_Speed );
    }
    if( pActionManager->DoAction("MoveLeft" ) )
    {
        Move( LEFT, m_Speed );
    }
    if( pActionManager->DoAction("MoveRight") )
    {
        Move( RIGHT, m_Speed );
    }
    if( pActionManager->DoAction("MoveUp") )
    {
        Move( UP, m_Speed );
    }
    if( pActionManager->DoAction("MoveDown") )
    {
        Move( DOWN, m_Speed );
    }
}

