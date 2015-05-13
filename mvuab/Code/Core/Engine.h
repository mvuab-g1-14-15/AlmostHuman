#ifndef ENGINE_H
#define ENGINE_H
#pragma once

#include <string>
#include <Windows.h>
#include "Utils\SingletonPattern.h"
#include "Utils\Defines.h"
#include "Process.h"

class CTimer;
class CLogRender;
class CEngineConfig;
class CEngineManagers;

class CEngine: public CSingleton<CEngine>
{
public:
  CEngine();
  ~CEngine();

  void Init( CEngineConfig* aEngineConfig );
  void SetRunnigProcess( CProcess* aProcess );

  void ProcessInputs();
  void Update();
  void Render();

  // Getters and setters
  GET_SET_PTR( CProcess, Process );
  GET_SET_PTR( CTimer, Timer );
  GET_SET_PTR( CEngineManagers, EngineManagers );
  GET_SET( bool, Play );

  void Trace( const std::string& TraceStr );

private:
  CProcess*           m_pProcess;
  CLogRender*         m_pLogRender;
  CEngineManagers*    m_pEngineManagers;
  CTimer*             m_pTimer;

  float               m_RenderTime;
  float               m_RenderTarget;

  bool                m_Play;
};

#endif // ENGINE_H
