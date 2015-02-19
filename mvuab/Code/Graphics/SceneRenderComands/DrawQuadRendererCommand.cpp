#include "Effects\EffectManager.h"
#include "GraphicsManager.h"
#include "SceneRenderComands\DrawQuadRendererCommand.h"
#include "Effects\Effect.h"
#include "Effects\EffectTechnique.h"
#include <windef.h>
#include "RenderableVertex\VertexTypes.h"

CDrawQuadRendererCommand::CDrawQuadRendererCommand( CXMLTreeNode& atts ):
  CStagedTexturedRendererCommand( atts )
{
}

CDrawQuadRendererCommand::~CDrawQuadRendererCommand()
{
}

void CDrawQuadRendererCommand::Execute( CGraphicsManager& GM )
{
  for ( size_t i = 0; i < m_StageTextures.size()  ; ++i )
  {
    const std::string& l_EffectTechName =
      CEffectManager::GetSingletonPtr()->GetTechniqueEffectNameByVertexDefault(
        SCREEN_COLOR_VERTEX::GetVertexType() );
    CEffectTechnique* l_EffectTech = CEffectManager::GetSingletonPtr()->GetResource( l_EffectTechName );
    uint32 width, height;
    GM.GetWidthAndHeight( width, height );
    RECT l_Rect = { 0, 0, ( long )width - 1, ( long )height - 1 };

    if ( l_EffectTech )
    {
      GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique( l_EffectTech, l_Rect, m_Color,
          m_StageTextures[i].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f, m_StageTextures[i].m_StageId );//TODO MIRAR SI ES NECESARIO PASAR EL STAGEID
    }
    else
    {
      GM.DrawColoredQuad2DTexturedInPixels( l_Rect, m_Color,
                                            m_StageTextures[i].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f, m_StageTextures[i].m_StageId );
    }
  }
}