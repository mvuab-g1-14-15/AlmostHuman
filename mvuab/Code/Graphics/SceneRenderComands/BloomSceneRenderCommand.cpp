#include "BloomSceneRenderCommand.h"
#include "BloomProcessSceneRendererCommand.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

// --------------------------------------------
//      CONSTRUCTOR/DESTRUCTOR
// --------------------------------------------
CBloomSceneRenderCommand::CBloomSceneRenderCommand( CXMLTreeNode& _Node )
  : CSceneRendererCommand( _Node )
{

  SBloomProcessSettings settings;

  settings.m_BloomThreshold = _Node.GetFloatProperty( "BloomThreshold", 0.25f );
  settings.m_BlurAmount   = _Node.GetFloatProperty( "BlurAmount", 4.0f );
  settings.m_BloomIntensity = _Node.GetFloatProperty( "BloomIntensity", 1.25f );
  settings.m_BaseIntensity  = _Node.GetFloatProperty( "BaseIntensity", 1.0f );
  settings.m_BloomSaturation  = _Node.GetFloatProperty( "BloomSaturation", 1.0f );
  settings.m_BloomSaturation  = _Node.GetFloatProperty( "BaseSaturation", 1.0f );

  m_Bloom = new CBloomProcessSceneRenderCommand( settings );
  m_Bloom->Init();
}

CBloomSceneRenderCommand::~CBloomSceneRenderCommand()
{
  CHECKED_DELETE( m_Bloom );
}

void CBloomSceneRenderCommand::Execute( CGraphicsManager& _GM )
{
  m_Bloom->Render();
}
