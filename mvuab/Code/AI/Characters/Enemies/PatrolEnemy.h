#pragma once

#ifndef _PATROL_ENEMY_H_
#define _PATROL_ENEMY_H_

#include <vector>
#include "Math\Vector3.h"
#include "Enemy.h"
#include "Utils\Defines.h"

class CPatrolEnemy : public CEnemy
{
private:
  std::vector<Math::Vect3f>                    m_Waypoints;
  int                                          m_CurrentPoint;
  size_t									   m_RouteId;
public:
  CPatrolEnemy( CXMLTreeNode& Node );
  ~CPatrolEnemy();
  void Init();
  void Update();
  void Render();

  int getCount()
  {
    return m_Waypoints.size();
  }
  GET_SET_REF( std::vector<Math::Vect3f>, Waypoints )
  GET_SET( int , CurrentPoint )
  GET_SET( int, RouteId)
};

#endif
