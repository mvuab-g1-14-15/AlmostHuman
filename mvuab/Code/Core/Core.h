#ifndef CORE_H
#define CORE_H
#pragma once

// Includes
#include <string>
#include <vector>
#include <Windows.h>

#include "Utils\SingletonPattern.h"
#include "Utils\Defines.h"
#include "Math\Vector3.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "Utils\Manager.h"

// Forward declarations
class CTimer;
class CConsole;

class CCore : public CSingleton<CCore>, public CTemplatedVectorMapManager<CManager>
{
    private:

        // Configuration

        // Managers
        CTimer*                             m_pTimer;
        CConsole*                           m_pConsole;

        void CreateManagers();
        void InitManagers();

    public:
        CCore();
        ~CCore();

        void Init();
        void Update();
        void Render();

        // Methods that trace Lua
        void Trace( const std::string& TraceStr );


        // Getters and setters
        GET_SET_PTR( CTimer, Timer );
};

#endif // GRAPHICS_MANAGER_H