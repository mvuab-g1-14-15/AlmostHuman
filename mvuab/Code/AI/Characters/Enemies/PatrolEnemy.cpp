#include "PatrolEnemy.h"
#include <stdio.h>

CPatrolEnemy::CPatrolEnemy( CXMLTreeNode& Node )
  : CEnemy( Node )
{
}


CPatrolEnemy::~CPatrolEnemy( void )
{
}

void CPatrolEnemy::Update( float32 deltaTime )
{
  CCharacter::Update();
  printf( "Updating CPatrolEnemy" );
}

void CPatrolEnemy::Render()
{
  CCharacter::Render();
  printf( "Rendering CPatrolEnemy" );
}