#include "Enemy.h"

CEnemy::CEnemy(CXMLTreeNode &Node)
    : m_Name(Node.GetPszProperty("name", ""))
    , m_Life(Node.GetFloatProperty("life", 0.0f))
    , m_Yaw(Node.GetFloatProperty("yaw", 0.0f))
    , m_Position(Node.GetVect3fProperty("pos", Math::Vect3f()))
{
}


CEnemy::~CEnemy()
{
}
