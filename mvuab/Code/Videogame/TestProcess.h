#ifndef TEST_PROCESS_H
#define TEST_PROCESS_H
#pragma once

#include "Process.h"
#include "Utils\Types.h"

#include <vector>


class CTestProcess : public CProcess
{
private:

public:
  CTestProcess();
  virtual ~CTestProcess();

  virtual void Init();
  virtual void Update();
  virtual void Render();
  virtual void RenderDebugInfo();
};

#endif // TEST_PROCESS_H
