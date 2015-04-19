#include "PatrolEnemy.h"
#include <stdio.h>
#include "GraphicsManager.h"

#include "EngineManagers.h"

CPatrolEnemy::CPatrolEnemy( CXMLTreeNode& Node, CStateMachine* aStateMachine )
    : CEnemy( Node, aStateMachine ), m_CurrentPoint( 0 )
    , m_RouteId(Node.GetIntProperty("route", -1))
{
}


CPatrolEnemy::~CPatrolEnemy( void )
{
    //Destroy();
}

void CPatrolEnemy::Init()
{
}

void CPatrolEnemy::Update( )
{
    CEnemy::Update();
    CCharacter::Update();
}

void CPatrolEnemy::Render()
{
    CCharacter::Render();

    CGraphicsManager* l_GM = GraphicsInstance;

    /*  std::vector<Vect3f>::iterator it = m_Waypoints.begin(),
                                  it_end = m_Waypoints.end();
        for (; it!=it_end; ++it)
        {
        Mat44f m;
        m.Translate(*it);
        l_GM->SetTransform(m);
        l_GM->DrawCube(0.2f);
        }

        Mat44f m;
        m.SetIdentity();
        l_GM->SetTransform(m);

        l_GM->DrawLine(GetPosition(), m_Waypoints[m_CurrentPoint]);*/
}