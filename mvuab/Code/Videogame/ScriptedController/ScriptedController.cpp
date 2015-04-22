#include "ScriptedController.h"
#include "Math\Vector3.h"

#include "GraphicsManager.h"

#include "EngineManagers.h"

CScriptedController::CScriptedController()
{
    SetPitch( 0.0f );
    SetRoll( 0.0f );
    SetYaw( 0.0f );
    SetPosition( Math::Vect3f( 0.0f, 0.0f, 0.0f ) );
    SetScale( Math::Vect3f( 1.0f, 1.0f, 1.0f ) );
    SetName( "CUBO" );
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
    GraphicsInstance->DrawCube( GetTransform(), 1.0f );
}