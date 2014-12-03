#include "EasyEnemy.h"
#include <stdio.h>

CEasyEnemy::CEasyEnemy(CXMLTreeNode &Node)
: CEnemy(Node)
{
}


CEasyEnemy::~CEasyEnemy()
{
}

void CEasyEnemy::Update(float32 deltaTime)
{
	printf("Updating CEasyEnemy");
}

void CEasyEnemy::Render()
{
	printf("Rendering CEasyEnemy");
}