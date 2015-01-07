#include "CameraFPShooter.h"
#include "ActionManager.h"
#include "Timer\Timer.h"

CCameraFPShooter::CCameraFPShooter()
    : m_Height( 2.0f ),
    m_YawSpeed(100.f),
    m_PitchSpeed( 60.0f),
    m_ForwardSpeed(10.f),
    m_StrafeSpeed(5.0f),
    m_Speed(2.f)
{
    SetCameraType(CCamera::FirstPerson);
}

CCameraFPShooter::~CCameraFPShooter()
{
}

void CCameraFPShooter::Move( int strafe, int forward, bool flag_speed, const float32 &dt)
{
    Math::Vect3f addPos;
    addPos.x =    forward * ( Math::Utils::Cos(m_Yaw) )+ strafe * (  Math::Utils::Cos(m_Yaw + Math::pi32 * 0.5f) );
    addPos.y =  0.0f;
    addPos.z =    forward * ( Math::Utils::Sin(m_Yaw) )+
                  strafe  * ( Math::Utils::Sin(m_Yaw+ Math::pi32 * 0.5f) );

    addPos = addPos.GetNormalized();
    float32 constant = dt * m_ForwardSpeed;
    if( flag_speed )
        constant *= m_Speed;
    addPos *= constant;
    m_Pos += addPos;
}

void CCameraFPShooter::Update( )
{
    CActionManager *pActionManager = CActionManager::GetSingletonPtr();
    float32 amount;
    bool flag_speed =  pActionManager->DoAction("Run");
    int forward = 0;
    int strafe = 0;
    if( pActionManager->DoAction("MoveForward") )
    {
        forward += 1;
        Move(strafe, forward ,flag_speed, deltaTime);
    }
    if( pActionManager->DoAction("MoveBackward") )
    {
        forward -= 1;
        Move(strafe, forward ,flag_speed, deltaTime);
    }
    if( pActionManager->DoAction("MoveLeft" ) )
    {
        strafe += 1;
        Move(strafe, forward ,flag_speed, deltaTime);
    }
    if( pActionManager->DoAction("MoveRight") )
    {
        strafe -= 1;
        Move(strafe, forward ,flag_speed, deltaTime);
    }

	if( pActionManager->DoAction("MoveUp" ) )
    {
        m_Pos.y +=1;
    }
    if( pActionManager->DoAction("MoveDown") )
    {
		m_Pos.y -=1;
    }

    //Definimos el movimiento del Yaw y Pitch de la camara
    if( pActionManager->DoAction("MoveYaw",amount) )
    {
        AddYaw( amount * deltaTime );
    }
    if( pActionManager->DoAction("MovePitch",amount) )
    {
        AddPitch( amount * deltaTime );
    }
}

void CCameraFPShooter::AddYaw( float32 degree )
{
    m_Yaw -= Math::Utils::Deg2Rad(degree * m_YawSpeed);
    
    //Para controlar que el valor de m_yaw siempre este acotado
    if( m_Yaw > Math::two_pi32)
        m_Yaw -= Math::two_pi32;
    else if( m_Yaw < -Math::two_pi32 )
        m_Yaw += Math::two_pi32;
}

void CCameraFPShooter::AddPitch( float32 radian )
{
    float32 degrees = Math::Utils::Deg2Rad(radian * m_PitchSpeed);
    //Hemos de limitar el ángulo yaw entre 90º  y -90º
    if( (m_Pitch - degrees <  (Math::pi32*0.5)) &&  
        (m_Pitch - degrees > -(Math::pi32*0.5)) )
    {    
        m_Pitch -= degrees;
    }
}

void CCameraFPShooter::AddHeight( float32 amount )
{
    m_Height += amount;
    m_Pos.y = m_Height;
}

Math::Vect3f CCameraFPShooter::GetLookAt(void) const
{    
    //Pasamos de coordenadas esfericas a coordenadas cartesianas
    Math::Vect3f look( Math::Utils::Cos(m_Yaw) * Math::Utils::Cos(m_Pitch), 
                       Math::Utils::Sin(m_Pitch),
                       Math::Utils::Sin(m_Yaw) * Math::Utils::Cos(m_Pitch) );

    return m_Pos + look;
}

Math::Vect3f CCameraFPShooter::GetVecUp(void) const
{
    Math::Vect3f vUpVec( -Math::Utils::Cos(m_Yaw) * Math::Utils::Sin(m_Pitch), 
                         Math::Utils::Cos(m_Pitch), 
                        -Math::Utils::Sin(m_Yaw) * Math::Utils::Sin(m_Pitch) );
    
    return vUpVec;
}