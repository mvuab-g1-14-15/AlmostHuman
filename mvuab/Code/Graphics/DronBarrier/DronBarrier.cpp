#include "DronBarrier.h"

// Core
#include "Utils/Defines.h"
#include "EngineManagers.h"

// Gph
#include "GraphicsManager.h"
#include "RenderableVertex/VertexTypes.h"
#include "RenderableVertex/RenderableVertex.h"
#include "RenderableVertex/IndexedVertexs.h"
#include "Effects/EffectTechnique.h"
#include "Effects/EffectManager.h"

CRenderableVertexs* CDronBarrier::sRV = 0;

//------------------------------------------------------------------------------------------------
CDronBarrier::CDronBarrier()
  : CObject3D()
  , mColor( Math::colBLUE )
  , mTechnique( EffectManagerInstance->GetEffectTechnique( "DronBarrierTechnique" ) )
{
  ASSERT( mTechnique, "Null technique to render the dron barrier" );
}

void CDronBarrier::SetColor( Math::CColor aColor )
{
  mColor = aColor;
}

//------------------------------------------------------------------------------------------------
CDronBarrier::~CDronBarrier()
{
}

void CDronBarrier::Render( )
{
  CGraphicsManager* lGM = GraphicsInstance;
  uint32 lWidth, lHeight;
  lGM->GetWidthAndHeight( lWidth, lHeight );
  lGM->SetTransform( GetTransform() );
  mTechnique->SetDebugColor( mColor );
  sRV->Render( lGM, mTechnique );
  lGM->SetTransform( Math::Mat44f() );
}
//------------------------------------------------------------------------------------------------
void CDronBarrier::CreateGeometry()
{
  ASSERT( sRV == 0, "Initing twice the geometry of the dron barrier" );
  const uint32 lIdxCount = 3;
  const uint32 lVtxCount = 3;

  TT1_VERTEX lVtx[lVtxCount] =
  {
    {  0.0f, 0.8f, 0.0f,  0.5f, 1.0f},
    { -0.8f, -0.8f, 0.0f, 0.0f, 0.0f},
    {  0.8f, -0.8f, 0.0f, 1.0f, 0.0f}
  };

  unsigned short int lIdx[lIdxCount] =
  {
    0, 1, 2    // side 1
  };

  sRV = new CIndexedVertexs<TT1_VERTEX, short>( GraphicsInstance, &lVtx, &lIdx, lVtxCount, lIdxCount );
}
//------------------------------------------------------------------------------------------------
void CDronBarrier::DestroyGeometry()
{
  ASSERT( sRV != 0, "Destroying a null geometry for dron barrier" );
  CHECKED_DELETE( sRV );
}
