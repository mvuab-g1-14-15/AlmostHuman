#ifndef TEST_PROCESS_H
#define TEST_PROCESS_H
#pragma once

#include "Process.h"
#include "Utils\Types.h"

#include "Artemis/Artemis.h"
#include "Artemis/World.h"
#include "Artemis/Entity.h"

#include <vector>

class CMovementSystem;

class CTestProcess : public CProcess
{
private:
    artemis::World* m_World;
    CMovementSystem* m_MovementSystem;
    artemis::Entity* m_Player;

public:
    CTestProcess();
    virtual ~CTestProcess();

    virtual void Init();
    virtual void Update();
    virtual void Render();
};

#endif // TEST_PROCESS_H
