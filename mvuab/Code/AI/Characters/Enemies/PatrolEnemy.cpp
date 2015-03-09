#include "PatrolEnemy.h"
#include <stdio.h>
#include "GraphicsManager.h"
#include "Core.h"

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

  CGraphicsManager* l_GM = CCore::GetSingletonPtr()->GetGraphicsManager();

  std::vector<Vect3f>::iterator it = m_Waypoints.begin(),
                                it_end = m_Waypoints.end();
  for (; it!=it_end; ++it)
  {
    Mat44f m;
    m.Translate(*it);
    l_GM->SetTransform(m);
    l_GM->DrawCube(0.2f);
  }

  Mat44f m;
  m.SetIdentity();
  l_GM->SetTransform(m);

  l_GM->DrawLine(GetPosition(), m_Waypoints[m_CurrentPoint]);
}