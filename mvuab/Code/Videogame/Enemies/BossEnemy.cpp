#include "BossEnemy.h"
#include <stdio.h>

CBossEnemy::CBossEnemy(CXMLTreeNode &Node)
: CEnemy(Node)
{
}


CBossEnemy::~CBossEnemy(void)
{
}

void CBossEnemy::Update(float32 deltaTime)
{
	printf("Updating bossenemy");
}

void CBossEnemy::Render()
{
	printf("Rendering bossenemy");
}
