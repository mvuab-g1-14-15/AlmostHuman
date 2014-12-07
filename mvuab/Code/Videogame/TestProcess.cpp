#include "TestProcess.h"
#include "Utils\Defines.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "ActionManager.h"
#include "Cameras\FPSCamera.h"
#include "Cameras\TPSCamera.h"
#include "Object3D.h"
#include "Math\Matrix44.h"
#include "Logger\Logger.h"
#include "Utils\LogRender.h"
#include "Utils\Defines.h"
#include "Math\AABB.h"

#include "RenderableVertex\RenderableVertex.h"
#include "RenderableVertex\IndexedVertexs.h"
#include "RenderableVertex\VertexTypes.h"
#include "Texture\Texture.h"

#include "StaticMeshes\StaticMesh.h"

#include <d3dx9.h>

CRenderableVertexs *g_RV = 0;
CTexture *g_TX = 0;
CStaticMesh *g_StaticMesh = 0;

CTestProcess::CTestProcess() : CProcess(), 
	m_Speed( 0.1f ),
	m_pFPSCamera( new CFPSCamera( Vect3f(15.0f,0.0f,0.0f), Vect3f(0.0f,0.0f,-0.0f), new CObject3D()) ),
	m_pTPSCamera( new CTPSCamera( Vect3f(15.0f,0.0f,0.0f), Vect3f(0.0f,0.0f,-0.0f), new CObject3D()) ),
	m_Amount( 0.0f ), m_Angle( 0.0f ),  m_AngleMoon( 0.0f ), m_PaintAll(false)
{
}

CTestProcess::~CTestProcess()
{
	CLogger::GetSingletonPtr()->SaveLogsInFile();
	CHECKED_DELETE(m_pFPSCamera);
	CHECKED_DELETE(m_pTPSCamera);
}

void CTestProcess::Update(float32 deltaTime)
{
	m_FPS = 1.0f/deltaTime;

	CInputManager* pInputManager = CInputManager::GetSingletonPtr();

	m_Amount +=  0.01f;
	m_Angle  += deltaTime * 0.05f;
	m_AngleMoon += deltaTime * 0.05f;
	
	Vect3f CameraDirection( m_pCamera->GetDirection() );

	CActionManager* pActionManager = CActionManager::GetSingletonPtr();

	if( pActionManager->DoAction("CommutationCamera") )
	{
		CFPSCamera* pProcessCamera = dynamic_cast<CFPSCamera*>(m_pCamera);
		if( pProcessCamera ) m_pCamera = m_pTPSCamera;
		else m_pCamera = m_pFPSCamera;
	}

	if( pActionManager->DoAction("MoveForward") )
	{
		m_pCamera->Move( CFPSCamera::FORWARD, m_Speed );
	}
	if( pActionManager->DoAction("MoveBackward") )
	{
		m_pCamera->Move( CFPSCamera::BACKWARD, m_Speed );
	}
	if( pActionManager->DoAction("MoveLeft" ) )
	{
		m_pCamera->Move( CFPSCamera::LEFT, m_Speed );
	}
	if( pActionManager->DoAction("MoveRight") )
	{
		m_pCamera->Move( CFPSCamera::RIGHT, m_Speed );
	}
	if( pActionManager->DoAction("MoveUp") )
	{
		m_pCamera->Move( CFPSCamera::UP, m_Speed );
	}
	if( pActionManager->DoAction("MoveDown") )
	{
		m_pCamera->Move( CFPSCamera::DOWN, m_Speed );
	}

	Vect3i delta = pInputManager->GetMouseDelta();
	if( pInputManager->IsDown( IDV_MOUSE, MOUSE_BUTTON_LEFT) )
	{
		m_pCamera->AddYawPitch((float) delta.x, (float) delta.y);
	}

	CTPSCamera* pTPSCam = dynamic_cast<CTPSCamera*>(m_pCamera);
	if(pTPSCam) pTPSCam->AddZoom(delta.z * m_Speed);
}

void CTestProcess::Init()
{
	m_pCamera = m_pFPSCamera;
	TT1_VERTEX v[4] =
	{
			{ 0.0f, 0.0f, 0.0f,	0.0f, 0.0f }
		,	{ 0.0f, 5.0f, 0.0f,	0.0f, 1.0f }
		,	{ 5.0f, 5.0f, 0.0f,	1.0f, 1.0f }
		,	{ 5.0f, 0.0f, 0.0f,	1.0f, 0.0f }
	};

	unsigned short int indexes[6] = { 0, 1, 2, 0, 2, 3};

	g_RV = new CIndexedVertexs<TT1_VERTEX>(CGraphicsManager::GetSingletonPtr(), v, indexes, 4, 6 );
	g_TX = new CTexture(); g_TX->Load("./Data/Textures/texture0.jpg");
	g_StaticMesh = new CStaticMesh();
	g_StaticMesh->Load("./Data/meshes/prueba.mesh");

    AABB3f BndBox(Vect3f(0.0f,0.0f,0.0f), Vect3f(0.0f,1.0f,1.0f) );

    Vect3f center( BndBox.GetCenter() );
    Vect3f height( BndBox.GetCenter() );
    Vect3f radious( BndBox.GetCenter() );
}

void CTestProcess::Render()
{
	CGraphicsManager* pGraphicsManager = GraphicsInstance;

	//pGraphicsManager->DrawAxis(5);
	pGraphicsManager->DrawGrid(100,colORANGE,50,50);

	Mat44f identity;
	identity.SetIdentity();

	//pGraphicsManager->DrawCylinder( 10,10,20,20, colBLACK, true);
	//pGraphicsManager->DrawSphere( 2, colYELLOW, 20 );
	pGraphicsManager->DrawIcoSphere();
	if( m_PaintAll )
	{
		// Sun
		Mat44f transformationSun;
		transformationSun.SetIdentity();
		transformationSun.RotByAngleY(m_Amount);
		pGraphicsManager->SetTransform( transformationSun );
		pGraphicsManager->DrawSphere( 2, colYELLOW, 20 );
		pGraphicsManager->SetTransform( identity );

		// Earth
		Mat44f transformationEarth;
		transformationEarth.SetIdentity();
		transformationEarth.RotByAngleY(m_Amount);

		Mat44f transformationEarthTrsf;
		transformationEarthTrsf.SetIdentity();
		transformationEarthTrsf.Translate( Vect3f( mathUtils::Cos(m_Angle) * 10.0f, 0.0f, mathUtils::Sin(m_Angle)* 10.0f ) );

		pGraphicsManager->SetTransform(  transformationEarth*transformationEarthTrsf );
		pGraphicsManager->DrawSphere( 1, colWHITE, 20 );
		pGraphicsManager->SetTransform( identity );

		//Moon
		Mat44f transformationMoon;
		transformationMoon.SetIdentity();
		transformationMoon.RotByAngleY(m_Amount);

		Mat44f transformationMoonTrsf;
		transformationMoonTrsf.SetIdentity();
		transformationMoonTrsf.Translate( Vect3f( mathUtils::Cos(m_AngleMoon)*2.0f, 0.0f, mathUtils::Sin(m_AngleMoon) * 2.0f ) );

		pGraphicsManager->SetTransform( transformationMoonTrsf * transformationMoon * transformationEarthTrsf );
		pGraphicsManager->DrawSphere( 0.2f, colBLACK, 20 );
	}

	pGraphicsManager->SetTransform( identity );

	g_TX->Activate(0);
	g_RV->Render(pGraphicsManager);

	//pGraphicsManager->DrawCamera(m_pTPSCamera);
	//pGraphicsManager->DrawCamera(m_pFPSCamera);
}

void CTestProcess::RenderDebugInfo()
{
	CProcess::RenderDebugInfo();
}
