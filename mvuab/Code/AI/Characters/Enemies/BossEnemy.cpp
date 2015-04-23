#include "BossEnemy.h"
#include <stdio.h>

CBossEnemy::CBossEnemy(CXMLTreeNode &Node, CStateMachine* aStateMachine)
: CEnemy(Node, aStateMachine)
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
