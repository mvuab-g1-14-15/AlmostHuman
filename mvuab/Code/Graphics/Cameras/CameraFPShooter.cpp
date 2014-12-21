#include "CameraFPShooter.h"
#include "ActionManager.h"

CCameraFPShooter::CCameraFPShooter()
{
  m_posY            = 2.f;
  m_Pos            = Math::Vect3f(0.f, m_posY, 0.f);
  m_yaw            = 0.f;
  m_pitch            = 0.f;
  m_view_d        =  2.f;
  m_fov_radians    = D3DXToRadian( 50.f );
  m_aspect_ratio    = 640.f / 480.f;
  m_zn            = 0.1f;
  m_zf            = 1000.f;
  m_speed_yaw        =  100.f;
  m_speed_pitch    = 60.f;
  m_speed_forward = 10.f;
  m_speed_strafe    = 5.f;
  m_speed            = 2.f;
}

CCameraFPShooter::CCameraFPShooter( const D3DXVECTOR3 &InitialPosition, const D3DXVECTOR3 &TargetPoint )
{
  m_posY            = InitialPosition.y;
  m_Pos             = Math::Vect3f(InitialPosition.x, InitialPosition.y, InitialPosition.z);

  D3DXVECTOR3 d = TargetPoint - InitialPosition;
  // Calculate the yaw and the pitch to allow the camera be looking at the TargetPoint

  m_yaw               = Math::Utils::ATan2(d.z, d.x);
  m_pitch             = Math::Utils::ATan2(d.y,Math::Utils::Sqrt(d.z * d.z + d.x * d.x));
  m_view_d        =  2.f;
  m_fov_radians    = D3DXToRadian( 50.f );
  m_aspect_ratio    = 640.f / 480.f;
  m_zn            = 0.1f;
  m_zf            = 1000.f;
  m_speed_yaw        =  100.f;
  m_speed_pitch    = 60.f;
  m_speed_forward = 10.f;
  m_speed_strafe    = 5.f;
  m_speed            = 2.f;
}


CCameraFPShooter::~CCameraFPShooter()
{
    
}

void CCameraFPShooter::Move( int strafe, int forward, bool flag_speed, const float32 &dt)
{
    Math::Vect3f addPos;
    addPos.y =  0.0f;
    addPos.x =    forward * (  cos(m_yaw) )+
                strafe  * (  cos(m_yaw+ D3DX_PI * 0.5f) );
                

    addPos.z =    forward * (  sin(m_yaw) )+
                strafe  * (  sin(m_yaw+ D3DX_PI * 0.5f) );
                
    
    addPos = addPos.GetNormalized();
    float32 constant = dt * m_speed_forward;
    if( flag_speed )
        constant *= m_speed;
    addPos *= constant;
    m_Pos += addPos;
}

void CCameraFPShooter::Update( float32 deltaTime )
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

void CCameraFPShooter::AddYaw( const float32 &degree ) 
{
    m_yaw -= D3DXToRadian(degree * m_speed_yaw);
    
    //Para controlar que el valor de m_yaw siempre este acotado
    if( m_yaw > 2*D3DX_PI)
        m_yaw -= 2*D3DX_PI;    
    else if( m_yaw < -2*D3DX_PI ) 
        m_yaw += 2*D3DX_PI;

}

void CCameraFPShooter::AddPitch( const float32 &radian ) 
{
    float32 degrees = D3DXToRadian(radian * m_speed_pitch);
    //Hemos de limitar el ángulo yaw entre 90º  y -90º
    if( (m_pitch - degrees <  (D3DX_PI*0.5)) &&  
        (m_pitch - degrees > -(D3DX_PI*0.5)) )
    {    
        m_pitch -= degrees;
    }
}

Math::Vect3f CCameraFPShooter::GetLookAt(void) const
{    
    //Pasamos de coordenadas esfericas a coordenadas cartesianas
    Math::Vect3f look( Math::Utils::Cos(m_yaw) * Math::Utils::Cos(m_pitch), 
                       Math::Utils::Sin(m_pitch),
                       Math::Utils::Sin(m_yaw) * Math::Utils::Cos(m_pitch) );

    return m_Pos + look;
}

Math::Vect3f CCameraFPShooter::GetVecUp(void) const
{
    Math::Vect3f vUpVec( -Math::Utils::Cos(m_yaw) * Math::Utils::Sin(m_pitch), 
                         Math::Utils::Cos(m_pitch), 
                        -Math::Utils::Sin(m_yaw) * Math::Utils::Sin(m_pitch) );
    
    return vUpVec;
}