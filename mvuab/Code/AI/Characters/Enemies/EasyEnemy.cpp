#include "EasyEnemy.h"
#include <stdio.h>

CEasyEnemy::CEasyEnemy( CXMLTreeNode& Node )
  : CEnemy( Node )
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