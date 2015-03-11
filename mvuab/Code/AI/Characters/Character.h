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
  ~CCharacter();
  virtual void Init();
  virtual void Init( CXMLTreeNode& Node );
  virtual void Update();
  virtual void Render();
  void Destroy();
  GET_SET( Math::Vect3f, TargetPosition );
  GET_SET( Math::Vect3f, TargetPositionOriginal );
  GET_SET( float32, Speed );
  GET_SET( float32, Life );
protected:
  virtual ECollisionGroup GetCollisionGroup();
  virtual CPhysicUserData* GetPhysicsUserData();
private: // Members
  std::string m_AIPath;
  CPhysicController* m_PController;
  Math::Vect3f m_TargetPosition;
  Math::Vect3f m_TargetPositionOriginal;
  float32       m_Speed;
  float32             m_Life;
  bool m_init;

  float m_Height;
  float m_Radius;

private: // Methods
  void ExecuteAI();
};

#endif // CHARACTER_H
