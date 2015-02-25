#include "SceneRenderComands\GaussianSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"
#include "Effects\EffectTechnique.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectTechnique.h"
#include "Core.h"

CGaussianSceneRendererCommand::CGaussianSceneRendererCommand(
  CXMLTreeNode& atts )
  : CStagedTexturedRendererCommand( atts )
{
	CGraphicsManager* l_GM = CGraphicsManager::GetSingletonPtr();
	m_nIteration = atts.GetIntProperty("nIteration", 0);
	m_Width = atts.GetIntProperty("texture_width", 0);
	m_Height = atts.GetIntProperty("texture_height", 0);
}

CGaussianSceneRendererCommand::~CGaussianSceneRendererCommand()
{
}

void CGaussianSceneRendererCommand::Execute( CGraphicsManager& GM )
{
	//ActivateTextures();

	//uint32 l_Width = m_StageTextures[0].m_Texture->GetWidth();
	//uint32 l_Height = m_StageTextures[0].m_Texture->GetHeight();
	//CTexture::TFormatType l_iFormatType = CTexture::TFormatType::A8R8G8B8;
	//CTexture* l_Texture = new CTexture();
 //   l_Texture->Create( "auxtexture1", l_Width, l_Height, 0, CTexture::RENDERTARGET, CTexture::DEFAULT, l_iFormatType );


	//for(size_t i = 0; i < m_nIteration; ++i)
	//{

	//	#ifdef _DEBUG
	//		if ( false ) // DEBUG
	//			DebugTextures();
	//	#endif

	//std::string l_EffectTechName = CCore::GetSingletonPtr()->GetRenderableObjectTechniqueManager()->GetRenderableObjectTechniqueNameByVertexType(SCREEN_COLOR_VERTEX::GetVertexType() );
 // RenderableObjectTechnique* l_ROT = CCore::GetSingletonPtr()->GetRenderableObjectTechniqueManager()->GetResource( l_EffectTechName );

 // CEffectTechnique* l_EffectTech =  l_ROT->GetEffectTechnique();
 // uint32 width, height;
 // GM.GetWidthAndHeight( width, height );
 // RECT l_Rect = { 0, 0, ( long )width - 1, ( long )height - 1 };

 // l_EffectTech->GetEffect()->SetLight( 0 );

 // if ( l_EffectTech )
 // {
 //   GM.DrawColoredQuad2DTexturedInPixelsByEffectTechnique( l_EffectTech, l_Rect, m_Color,
 //       m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
 // }
 // else
 // {
 //   GM.DrawColoredQuad2DTexturedInPixels( l_Rect, m_Color,
 //                                         m_StageTextures[0].m_Texture, 0.0f, 0.0f, 1.0f, 1.0f );
 // }
	//}

  
}