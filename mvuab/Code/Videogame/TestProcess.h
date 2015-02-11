#ifndef TEST_PROCESS_H
#define TEST_PROCESS_H

#include "Process.h"

class CTestProcess : public CProcess
{
public:
    CTestProcess();
    virtual ~CTestProcess();

    virtual void Init();
    virtual void Update();
    virtual void Render();
    virtual void RenderDebugInfo();
};

#endif // TEST_PROCESS_H
