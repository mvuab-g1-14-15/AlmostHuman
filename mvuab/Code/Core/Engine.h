#ifndef ENGINE_H
#define ENGINE_H
#pragma once

#include <string>
#include <Windows.h>
#include "Utils\SingletonPattern.h"
#include "Utils\Defines.h"

class CCore;
class CProcess;
class CLogRender;
class CTimer;

class CEngine
{
public:
    CEngine();
    ~CEngine();
    void Init( CProcess * apProcess, const std::string &aConfigPath, HWND aWindowId );
    void Update();
    void Render();
    
    // Getters and setters
    GET_SET(CCore*, pCore);

private:
    CCore*              m_pCore;
    CLogRender*         m_pLogRender;
    CProcess*           m_pProcess;
    CTimer*             m_timer;
};

#endif // ENGINE_H
