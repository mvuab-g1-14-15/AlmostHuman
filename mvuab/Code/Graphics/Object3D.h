//----------------------------------------------------------------------------------
// CObject3D class
// Author: Enric Vergara
//
// Description:
// Esta clase será la base de toda instancia que pueda existir en la escena
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_OBJECT_3D_H_
#define INC_OBJECT_3D_H_

#include "Math\MathTypes.h"
#include "Math\Matrix44.h"
#include "Math\Vector3.h"

class CXMLTreeNode;

class CObject3D
{
protected:
  Math::Vect3f   m_Position;
  Math::Vect3f   m_Scale;

  float32       m_fYaw;
  float32       m_fPitch;
  float32       m_fRoll;
public:
  CObject3D();
  CObject3D( CXMLTreeNode& atts );
  CObject3D( const Math::Vect3f& pos, float32 yaw, float32 pitch, float32 roll = 0, Math::Vect3f scale = Math::Vect3f( 1.0f, 1.0f, 1.0f ) );
  
  virtual ~CObject3D( void )
  {
    /*Nothing*/;
  }

  //---Get Functions
  float32 GetYaw() const
  {
    return m_fYaw;
  }
  float32 GetRoll() const
  {
    return m_fRoll;
  }
  float32 GetPitch() const
  {
    return m_fPitch;
  }
  const Math::Vect3f& GetPosition() const
  {
    return m_Position;
  }
  const Math::Vect3f& GetScale() const
  {
    return m_Scale;
  }
  Math::Mat44f GetTransform();

  //---Set Functions
  void SetPosition( const Math::Vect3f& pos )
  {
    m_Position = pos;
  }
  void SetYaw( float32 yaw )
  {
    m_fYaw = yaw;
  }
  void SetPitch( float32 pitch )
  {
    m_fPitch = pitch;
  }
  void SetRoll( float32 roll )
  {
    m_fRoll = roll;
  }
  void SetScale( const Math::Vect3f& scale )
  {
    m_Scale = scale;
  }
};

#endif //INC_CORE_H_