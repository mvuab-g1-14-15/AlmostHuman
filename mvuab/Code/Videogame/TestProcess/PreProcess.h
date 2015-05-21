#ifndef _PRE_PROCESS_H
#define _PRE_PROCESS_H
#pragma once

#include "Process.h"
#include "PhysicsDefs.h"
#include "Utils\Types.h"
#include "Math\Vector3.h"

#include <vector>

//class CBillboard;

class CPreProcess : public CProcess
{
    private:

    public:
        CPreProcess();
        virtual ~CPreProcess();

        virtual void Init();
        virtual void Update();
        virtual void Render();
        virtual void RenderDebugInfo();

};

#endif // _PRE_PROCESS_H
