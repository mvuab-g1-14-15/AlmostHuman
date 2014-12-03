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
#include "RenderableVertex\RenderableVertex.h"
#include "RenderableVertex\VertexTypes.h"

#include <d3dx9.h>

struct TT1_VERTEX
{
        float x, y, z;
        float tu, tv;

        static inline unsigned short GetVertexType()
        {
                return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_TEXTURE1;
        }
        static inline unsigned int GetFVF()
        {
                return D3DFVF_XYZ|D3DFVF_TEX1;
        }
};

class CRenderableVertexs
{
public:
	CRenderableVertexs()
	{
	}
	virtual ~CRenderableVertexs()
	{
		CHECKED_RELEASE(m_VB);
		CHECKED_RELEASE(m_IB);
	}
	virtual bool Render(CGraphicsManager *RM) = 0;
	virtual inline size_t GetFacesCount() const{ return m_VertexCount / 3; }
	virtual inline size_t GetVertexsCount() const{ return m_VertexCount; }
	virtual inline unsigned short GetVertexType() const = 0;
	virtual inline size_t GetVertexSize() = 0;
	virtual inline size_t GetIndexSize() = 0;
protected:
	LPDIRECT3DVERTEXBUFFER9 m_VB;
	LPDIRECT3DINDEXBUFFER9 m_IB;
	size_t m_IndexCount, m_VertexCount;
};

template<class T>
class CIndexedVertexs : public CRenderableVertexs
{
protected:
	inline size_t GetVertexSize() {return sizeof(T);}
	inline size_t GetIndexSize() {return sizeof(unsigned short);}
	LPDIRECT3DTEXTURE9 m_Texture;
public:
	CIndexedVertexs(CGraphicsManager *renderManager, void *VertexAddress, void *IndexAddres, size_t
					VertexCount, size_t IndexCount)
	{
		m_VertexCount = VertexCount;
		m_IndexCount = IndexCount;
		const LPDIRECT3DDEVICE9 device = renderManager->GetDevice();
		
		device->CreateVertexBuffer( m_VertexCount* GetVertexSize() , 0, T::GetFVF(), D3DPOOL_DEFAULT, &m_VB, 0 );
		VOID* l_VRAMAdress = NULL;
		m_VB->Lock(0, m_VertexCount* GetVertexSize(), &l_VRAMAdress, 0);
		memcpy(l_VRAMAdress, VertexAddress, m_VertexCount* GetVertexSize());
		m_VB->Unlock();
		
		device->CreateIndexBuffer( m_IndexCount * GetIndexSize(), 0, D3DFMT_INDEX16,D3DPOOL_DEFAULT, &m_IB, 0 );
		l_VRAMAdress = NULL;
		m_IB->Lock(0, m_IndexCount * GetIndexSize(), &l_VRAMAdress, 0);
		memcpy(l_VRAMAdress, IndexAddres, m_IndexCount * GetIndexSize() );
		m_IB->Unlock();

		D3DXCreateTextureFromFile(device, "Data/textures/texture0.jpg", &m_Texture );
	}

	~CIndexedVertexs()
	{
	}

	virtual bool Render(CGraphicsManager *renderManager)
	{
		const LPDIRECT3DDEVICE9 device = renderManager->GetDevice();
		device->SetTexture(0, m_Texture);
		device->SetStreamSource(0, m_VB, 0, GetVertexSize() );
		device->SetIndices(m_IB);
		device->SetFVF(T::GetFVF());
		//device->DrawPrimitive(D3DPT_TRIANGLELIST,0,GetVertexsCount());
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,m_VertexCount,0, m_IndexCount/3 );
		return true;
	}
	virtual inline unsigned short GetVertexType() const {return T::GetVertexType();}
};

CRenderableVertexs* g_RV = 0;

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
	m_Angle  += deltaTime* 0.05f;
	m_AngleMoon += deltaTime*0.05;
	
	Vect3f CameraDirection( m_pCamera->GetDirection() );

	CActionManager* pActionManager = CActionManager::GetSingletonPtr();

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
	}

	if( pActionManager->DoAction("MoveForward" ) )
	{
		m_pCamera->Move( CFPSCamera::FORWARD, m_Speed );
	}
	if( pActionManager->DoAction("MoveBackward" ) )
	{
		m_pCamera->Move( CFPSCamera::BACKWARD, m_Speed );
	}
	if( pActionManager->DoAction("MoveLeft" ) )
	{
		m_pCamera->Move( CFPSCamera::LEFT, m_Speed );
	}
	if( pActionManager->DoAction("MoveRight" ) )
	{
		m_pCamera->Move( CFPSCamera::RIGHT, m_Speed );
	}
	if( pActionManager->DoAction("MoveUp" ) )
	{
		m_pCamera->Move( CFPSCamera::UP, m_Speed );
	}
	if( pActionManager->DoAction("MoveDown" ) )
	{
		m_pCamera->Move( CFPSCamera::DOWN, m_Speed );
	}

	Vect3i delta = pInputManager->GetMouseDelta();
	if( pInputManager->IsDown( IDV_MOUSE, MOUSE_BUTTON_LEFT) )
	{
		m_pCamera->AddYawPitch(delta.x,delta.y);
	}

	CTPSCamera* pTPSCam = dynamic_cast<CTPSCamera*>(m_pCamera);
	if(pTPSCam)
		pTPSCam->AddZoom(delta.z * m_Speed);
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

	g_RV->Render(pGraphicsManager);

	//pGraphicsManager->DrawCamera(m_pTPSCamera);
	//pGraphicsManager->DrawCamera(m_pFPSCamera);
}

void CTestProcess::RenderDebugInfo()
{
	CProcess::RenderDebugInfo();
}