#ifndef CHARACTER_H
#define CHARACTER_H
#pragma once

#include "Utils\Defines.h"
#include "Object3D.h"
#include "Utils\Name.h"
#include <string>
#include "XML\XMLTreeNode.h"
#include "PhysicsDefs.h"

class CPhysicController;
class CPhysicUserData;

class CCharacter : public CObject3D, public CName
{
public:
  CCharacter( const std::string& Name );
  ~CCharacter() {};
  virtual void Init();
  virtual void Init( CXMLTreeNode& Node );
  virtual void Update();
  virtual void Render();

  void Move( Math::Vect3f, float);

  GET_SET( Math::Vect3f, TargetPosition );
  GET_SET( Math::Vect3f, TargetPositionOriginal );
  GET_SET( float32, Speed );
  GET_SET( float32, Life );
  GET_SET( float, TimeToShoot );
  GET_SET( float, MaxTimeToShoot );

  void AddDamage( float32 aDamage )
  {
    m_Life -= aDamage;
  }

  Math::Vect3f GetPosition()
  {
    return m_Position;
  }
  float GetRadius();

  float GetHeight();
protected:
  virtual ECollisionGroup GetCollisionGroup();

  CPhysicController*    m_Controller;
private: // Members
  std::string           m_AIPath;
  Math::Vect3f          m_TargetPosition;
  Math::Vect3f          m_TargetPositionOriginal;
  float32               m_Speed;
  float32               m_Life;

  float m_TimeToShoot;
  float m_MaxTimeToShoot;

  float m_Height;
  float m_Radius;

private: // Methods
  void ExecuteAI();
};

#endif // CHARACTER_H
