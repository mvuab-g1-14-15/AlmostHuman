#include "Enemy.h"

CEnemy::CEnemy(CXMLTreeNode &Node)
	: m_Name(Node.GetPszProperty("name", ""))
	, m_Life(Node.Getfloat32Property("life", 0.0f))
	, m_Yaw(Node.Getfloat32Property("yaw", 0.0f))
	, m_Position(Node.GetVect3fProperty("pos", Vect3f()))
{
}


CEnemy::~CEnemy()
{
}
