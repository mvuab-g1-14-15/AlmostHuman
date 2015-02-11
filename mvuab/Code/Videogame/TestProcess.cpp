#include "TestProcess.h"

#include "Entity.h"
#include "Component.h"
#include "Transform.h"

CTestProcess::CTestProcess() : CProcess()
{
}

CTestProcess::~CTestProcess()
{
}

void CTestProcess::Update()
{
}

void CTestProcess::Init()
{
  CEntity* l_Entity = new CEntity();
  CComponent* l_Component = new CTransform();

  int l_Type = l_Entity->GetEntityType();

  l_Entity->AddComponent(l_Component);

  l_Type = l_Entity->GetEntityType();

  int a = 1;
}

void CTestProcess::Render()
{
}

void CTestProcess::RenderDebugInfo()
{
}