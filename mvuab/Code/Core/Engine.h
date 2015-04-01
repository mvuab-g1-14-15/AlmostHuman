#ifndef ENGINE_H
#define ENGINE_H
#pragma once

#include <string>
#include <Windows.h>
#include "Utils\SingletonPattern.h"
#include "Utils\Defines.h"
#include "Process.h"

class CCore;
class CLogRender;
class CEngineConfig;

class CEngine: public CSingleton<CEngine>
{
public:
  CEngine();
  ~CEngine();
  void Init( CProcess* apProcess, const std::string& aConfigPath, HWND aWindowId );
  void Update();
  void Render();

  // Getters and setters
  GET_SET_PTR( CCore, Core );
  GET_SET_PTR( CProcess, Process );
  GET_SET_PTR( CEngineConfig, EngineConfig );

private:
  CCore*              m_pCore;
  CLogRender*         m_pLogRender;
  CProcess*           m_pProcess;
  CEngineConfig*      m_pEngineConfig;
};

#endif // ENGINE_H
