#include "Enemy.h"

CEnemy::CEnemy(CXMLTreeNode &Node)
	: CCharacter(Node.GetPszProperty("name", "no_name") )
{
	CCharacter::Init(Node);
}


CEnemy::~CEnemy()
{
}
