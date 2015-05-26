#include "Shapes.h"

// Core
#include "Utils/Defines.h"
#include "EngineManagers.h"

// Gph
#include "GraphicsManager.h"
#include "RenderableVertex/VertexTypes.h"
#include "RenderableVertex/RenderableVertex.h"
#include "RenderableVertex/IndexedVertexs.h"
#include "Effects/EffectTechnique.h"

CRenderableVertexs* CBoxShape::sRV = 0;

//------------------------------------------------------------------------------------------------
CShape::CShape()
	: CObject3D()
	, mTechnique( 0 )
{

}

//------------------------------------------------------------------------------------------------
CShape::~CShape()
{

}

//------------------------------------------------------------------------------------------------
CBoxShape::CBoxShape()
	: CShape()
{

}

//------------------------------------------------------------------------------------------------
CBoxShape::~CBoxShape()
{

}

void CBoxShape::Render( CEffectTechnique* aTechnique )
{
	ASSERT(aTechnique, "Null technique to render the box shape");
	CGraphicsManager* lGM = GraphicsInstance;
    lGM->SetTransform( GetTransform() );
    sRV->Render(lGM, mTechnique);
    lGM->SetTransform( Math::Mat44f() );
}
//------------------------------------------------------------------------------------------------
void CBoxShape::CreateGeometry()
{
	ASSERT(sRV == 0, "Initing twice the geometry of the box shape");

    const uint32 lIdxCount = 36;
    const uint32 lVtxCount = 8;

    TDIFF_VERTEX lVtx[lVtxCount] =
    {
			{ -0.5f, 0.5f, -0.5f },
			{ 0.5f, 0.5f, -0.5f  },
			{ -0.5f, -0.5f, -0.5f},
			{ 0.5f, -0.5f, -0.5f },
			{ -0.5f, 0.5f, 0.5f  },
			{ 0.5f, 0.5f, 0.5f   },
			{ -0.5f, -0.5f, 0.5f },
			{ 0.5f, -0.5f, 0.5f  },
	};

    unsigned short int lIdx[lIdxCount] =
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 0, 6,    // side 2
		6, 0, 2,
		7, 5, 6,    // side 3
		6, 5, 4,
		3, 1, 7,    // side 4
		7, 1, 5,
		4, 5, 0,    // side 5
		0, 5, 1,
		3, 7, 2,    // side 6
		2, 7, 6,
	};

    sRV = new CIndexedVertexs<TDIFF_VERTEX>(GraphicsInstance, &lVtx, &lIdx, lVtxCount, lIdxCount);
}
//------------------------------------------------------------------------------------------------
void CBoxShape::DestroyGeometry()
{
	ASSERT(sRV != 0, "Destroying a null geometry for box shape");
    CHECKED_DELETE(sRV);
}