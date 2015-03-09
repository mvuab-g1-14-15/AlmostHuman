#include "PatrolEnemy.h"
#include <stdio.h>

CPatrolEnemy::CPatrolEnemy( CXMLTreeNode& Node )
    : CEnemy( Node ), m_CurrentPoint(0)
{
}


CPatrolEnemy::~CPatrolEnemy( void )
{
}

void CPatrolEnemy::Init()
{
  CCharacter::Init();
}

void CPatrolEnemy::Update( )
{
  CEnemy::Update();
  /*SetTargetPosition(m_Waypoints[m_CurrentPoint]);
  SetTargetPositionOriginal(m_Waypoints[m_CurrentPoint]);*/
  CCharacter::Update();
}

void CPatrolEnemy::Render()
{
  CCharacter::Render();
}