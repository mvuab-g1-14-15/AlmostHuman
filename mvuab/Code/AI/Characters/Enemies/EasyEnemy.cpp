#include "EasyEnemy.h"
#include <stdio.h>

CEasyEnemy::CEasyEnemy( CXMLTreeNode& Node, CStateMachine* aStateMachine )
  : CEnemy( Node, aStateMachine )
{
}


CEasyEnemy::~CEasyEnemy()
{
}

void CEasyEnemy::Update( )
{
  CCharacter::Update();
}

void CEasyEnemy::Render()
{
}