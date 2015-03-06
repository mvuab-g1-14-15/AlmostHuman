#include "PatrolEnemy.h"
#include <stdio.h>

CPatrolEnemy::CPatrolEnemy( CXMLTreeNode& Node )
  : CEnemy( Node )
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
}

void CPatrolEnemy::Render()
{
  CCharacter::Render();
}