#include "TestProcess.h"

#include "Entity.h"
#include "Component.h"
#include "Transform.h"
#include "Renderer.h"

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

  int l_Type = l_Entity->GetEntityType();

  CComponent* l_Transform = new CTransform();
  l_Entity->AddComponent(l_Transform);

  l_Type = l_Entity->GetEntityType();

  CComponent* l_Renderer = new CRenderer();
  l_Entity->AddComponent(l_Renderer);

  l_Type = l_Entity->GetEntityType();

  l_Entity->RemoveComponent(TRANSFORM);

  l_Type = l_Entity->GetEntityType();

  int a = 1;
}

void CTestProcess::Render()
{
}

void CTestProcess::RenderDebugInfo()
{
}