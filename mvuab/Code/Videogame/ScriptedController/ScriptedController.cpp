#include "ScriptedController.h"
#include "Math\Vector3.h"

#include "GraphicsManager.h"
#include "Core.h"

CScriptedController::CScriptedController()
{
    SetPitch(0.0f);
    SetRoll(0.0f);
    SetYaw(0.0f);

    SetPosition(Math::Vect3f(0.0f, 0.0f, 0.0f));
    SetScale(Math::Vect3f(1.0f, 1.0f, 1.0f));

    SetName("CUBO");
}

CScriptedController::~CScriptedController()
{
}

void CScriptedController::Init()
{
}

void CScriptedController::Update()
{
}

void CScriptedController::Render()
{
    CCore::GetSingletonPtr()->GetGraphicsManager()->SetTransform(GetTransform());
    CCore::GetSingletonPtr()->GetGraphicsManager()->DrawCube(1.0f);
}
