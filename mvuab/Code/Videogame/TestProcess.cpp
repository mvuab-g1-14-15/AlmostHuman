#include "TestProcess.h"

#include "Utils/Defines.h"
#include "Math/Vector3.h"

#include "Entity-Component-System/Systems/Movement.h"
#include "Entity-Component-System/Components.h"

#include "Timer/Timer.h"

CTestProcess::CTestProcess()
    : CProcess()
{
}

CTestProcess::~CTestProcess()
{
}

void CTestProcess::Update()
{
    CTransform* comp = ( CTransform* )m_Player->getComponent<CTransform>();

    m_World->loopStart();
    m_World->setDelta( deltaTime );
    m_MovementSystem->process();

    std::cout << "Y:" << comp->m_Position.y << std::endl;
}

void CTestProcess::Init()
{
    m_World = new artemis::World();
    artemis::SystemManager* sm = m_World->getSystemManager();
    m_MovementSystem = ( CMovementSystem* )sm->setSystem( new CMovementSystem() );
    artemis::EntityManager* em = m_World->getEntityManager();

    sm->initializeAll();

    artemis::Entity& player = em->create();

    player.addComponent( new CTransform( Math::Vect3f( 0.0f ), Math::Vect3f( 0.0f ), Math::Vect3f( 1.0f ) ) );
    player.addComponent( new CVelocity( Math::Vect3f( 0.0f, 1.0f, 0.0f ), 1.0f ) );
    player.refresh();

    m_Player = &player;
}

void CTestProcess::Render()
{
}