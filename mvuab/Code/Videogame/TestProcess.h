#ifndef TEST_PROCESS_H
#define TEST_PROCESS_H
#pragma once

#include "Process.h"
#include <vector>
#include "Utils\Types.h"
#include "Math\Vector3.h"

class CPhysicActor;
class CPhysicUserData;
class CPhysicSphericalJoint;
class CPhysicRevoluteJoint;

class CTestProcess : public CProcess
{
private:
    float32                         m_Speed;
    float32                         m_Amount;
    float32                         m_Angle;
    float32                         m_AngleMoon;
    Math::Vect2i                    m_LastMousePoint;
    bool                            m_PaintAll;
    std::vector<CPhysicActor *>     m_vPA;
    std::vector<CPhysicUserData *>  m_vPUD;
    CPhysicSphericalJoint*          m_PSJ;
    CPhysicRevoluteJoint*           m_PRJ;

public:
    CTestProcess();
    virtual ~CTestProcess();

    virtual void Init();
    virtual void Update();
    virtual void Render();
    virtual void RenderDebugInfo();
};

#endif // TEST_PROCESS_H
