#ifndef _ENEMY_H
#define _ENEMY_H

#pragma once

#include "Math\Vector3.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"
#include "Characters\Character.h"

class CEnemy : public CCharacter
{
public:
  typedef enum { eEasy = 0, ePatroll, eBoss } EEnemyType;
  CEnemy(CXMLTreeNode &Node);
  virtual ~CEnemy();
  virtual void Update(){};
  virtual void Render(){};
protected:
  EEnemyType m_Type;
};

#endif

