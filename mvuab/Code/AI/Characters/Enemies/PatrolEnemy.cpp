#include "PatrolEnemy.h"
#include <stdio.h>

CPatrolEnemy::CPatrolEnemy(CXMLTreeNode &Node)
: CEnemy(Node)
{
}


CPatrolEnemy::~CPatrolEnemy(void)
{
}

void CPatrolEnemy::Update(float32 deltaTime)
{
    printf("Updating CPatrolEnemy");
}

void CPatrolEnemy::Render()
{
    printf("Rendering CPatrolEnemy");
}