#include "BossEnemy.h"
#include <stdio.h>

CBossEnemy::CBossEnemy(CXMLTreeNode &Node)
: CEnemy(Node)
{
}


CBossEnemy::~CBossEnemy(void)
{
}

void CBossEnemy::Update()
{
	CCharacter::Update();
}

void CBossEnemy::Render()
{
}
