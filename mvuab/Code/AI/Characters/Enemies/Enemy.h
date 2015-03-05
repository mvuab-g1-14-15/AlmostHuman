#ifndef _ENEMY_H
#define _ENEMY_H

#pragma once

#include "Math\Vector3.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"
#include "Characters\Character.h"

#include <string>

class CStateMachine;

class CEnemy : public CCharacter
{
public:
  typedef enum { eEasy = 0, ePatroll, eBoss } EEnemyType;
  CEnemy(CXMLTreeNode &Node);
  virtual ~CEnemy();
  virtual void Init();
  virtual void Update();
  virtual void Render();

  void ChangeState(std::string NewState);
protected:
  EEnemyType m_Type;

  // State Machine instance
  CStateMachine* m_pStateMachine;
  std::string m_CurrentState;
};

#endif

