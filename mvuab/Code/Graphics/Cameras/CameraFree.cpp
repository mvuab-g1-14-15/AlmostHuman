#include "CameraFree.h"
#include "ActionManager.h"
#include "Core.h"
#include "Timer\Timer.h"

CCameraFree::CCameraFree()
  : m_Height( 2.0f ),
    m_YawSpeed(100.f),
    m_PitchSpeed( 60.0f),
    m_ForwardSpeed(10.f),
    m_StrafeSpeed(5.0f),
    m_Speed(2.f)
{
  SetCameraType(CCamera::Free);
}

CCameraFree::~CCameraFree()
{
}

void CCameraFree::Move( int strafe, int forward, bool flag_speed, const float32 &dt)
{
  Math::Vect3f addPos;
  addPos.x =    forward * ( Math::Utils::Cos(m_fYaw) )+ strafe * (  Math::Utils::Cos(m_fYaw + Math::pi32 * 0.5f) );
  addPos.y =  0.0f;
  addPos.z =    forward * ( Math::Utils::Sin(m_fYaw) )+
                strafe  * ( Math::Utils::Sin(m_fYaw+ Math::pi32 * 0.5f) );

  addPos = addPos.GetNormalized();
  float32 constant = dt * m_ForwardSpeed;
  if( flag_speed )
    constant *= m_Speed;
  addPos *= constant;
  m_Position += addPos;
}

void CCameraFree::Update( )
{
}

void CCameraFree::AddYaw( float32 degree )
{
  m_fYaw -= Math::Utils::Deg2Rad(degree * m_YawSpeed);

  //Para controlar que el valor de m_yaw siempre este acotado
  if( m_fYaw > Math::two_pi32)
    m_fYaw -= Math::two_pi32;
  else if( m_fYaw < -Math::two_pi32 )
    m_fYaw += Math::two_pi32;
}

void CCameraFree::AddPitch( float32 radian )
{
  float32 degrees = Math::Utils::Deg2Rad(radian * m_PitchSpeed);
  //Hemos de limitar el ángulo yaw entre 90º  y -90º
  if( (m_fPitch - degrees <  (Math::pi32*0.5)) &&
      (m_fPitch - degrees > -(Math::pi32*0.5)) )
    m_fPitch -= degrees;
}

void CCameraFree::AddHeight( float32 amount )
{
  m_Height += amount;
  m_Position.y = m_Height;
}

Math::Vect3f CCameraFree::GetLookAt(void) const
{
  //Pasamos de coordenadas esfericas a coordenadas cartesianas
  Math::Vect3f look( Math::Utils::Cos(m_fYaw) * Math::Utils::Cos(m_fPitch),
                     Math::Utils::Sin(m_fPitch),
                     Math::Utils::Sin(m_fYaw) * Math::Utils::Cos(m_fPitch) );

  return m_Position + look;
}

Math::Vect3f CCameraFree::GetVecUp(void) const
{
  Math::Vect3f vUpVec( -Math::Utils::Cos(m_fYaw) * Math::Utils::Sin(m_fPitch),
                       Math::Utils::Cos(m_fPitch),
                       -Math::Utils::Sin(m_fYaw) * Math::Utils::Sin(m_fPitch) );

  return vUpVec;
}