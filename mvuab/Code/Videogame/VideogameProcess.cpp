#include "VideogameProcess.h"
#include "Utils\Defines.h"
#include "GraphicsManager.h"
#include "ActionManager.h"
#include "InputManager.h"
#include "Cameras\FPSCamera.h"
#include "Cameras\TPSCamera.h"
#include "Object3D.h"

CVideogameProcess::CVideogameProcess() : CProcess(), 
	m_Speed( 0.1f ),
	m_pFPSCamera( new CFPSCamera( Vect3f(15.0f,0.0f,0.0f), Vect3f(0.0f,0.0f,-0.0f), new CObject3D()) ),
	m_pTPSCamera( new CTPSCamera( Vect3f(15.0f,0.0f,0.0f), Vect3f(0.0f,0.0f,-0.0f), new CObject3D()) )
{
	
}

CVideogameProcess::~CVideogameProcess()
{
}

void CVideogameProcess::Update(float32 deltaTime)
{
}

void CVideogameProcess::Init()
{
	m_pCamera = m_pFPSCamera;
}

void CVideogameProcess::Render()
{
	CGraphicsManager* pGraphicsManager = GraphicsInstance;
	CInputManager* pInputManager = CInputManager::GetSingletonPtr();
	CActionManager* pActionManager = CActionManager::GetSingletonPtr();


	pGraphicsManager->DrawAxis(5);
	pGraphicsManager->DrawBox(2,10,15, colBLACK );
	pGraphicsManager->DrawGrid(40,colORANGE,20,20);
	pGraphicsManager->DrawSphere( 5, colRED, 80 );

	Vect3f CameraDirection( m_pCamera->GetDirection() );
	if( pActionManager->DoAction("CommutationCamera" ) )
	{
		CFPSCamera* pProcessCamera = dynamic_cast<CFPSCamera*>(m_pCamera);
		if( pProcessCamera )
		{
			m_pCamera = m_pTPSCamera;
		}
		else
		{
			m_pCamera = m_pFPSCamera;
		}
		return;
	}

	if( pInputManager->IsDown( IDV_KEYBOARD, KEY_W ) )
	{
		m_pCamera->Move( CFPSCamera::FORWARD, m_Speed );
	}
	if( pInputManager->IsDown( IDV_KEYBOARD, KEY_S ) )
	{
		m_pCamera->Move( CFPSCamera::BACKWARD, m_Speed );
	}
	if( pInputManager->IsDown( IDV_KEYBOARD, KEY_A ) )
	{
		m_pCamera->Move( CFPSCamera::LEFT, m_Speed );
	}
	if( pInputManager->IsDown( IDV_KEYBOARD, KEY_D ) )
	{
		m_pCamera->Move( CFPSCamera::RIGHT, m_Speed );
	}
	if( pInputManager->IsDown( IDV_KEYBOARD, KEY_Q ) )
	{
		m_pCamera->Move( CFPSCamera::UP, m_Speed );
	}
	if( pInputManager->IsDown( IDV_KEYBOARD, KEY_E ) )
	{
		m_pCamera->Move( CFPSCamera::DOWN, m_Speed );
	}

	if( pInputManager->IsDown( IDV_MOUSE, MOUSE_BUTTON_LEFT ) )
	{
		Vect3i delta = pInputManager->GetMouseDelta();
		m_pCamera->AddYawPitch(delta.x,delta.y);
	}
}