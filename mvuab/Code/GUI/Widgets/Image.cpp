#include "Widgets\Image.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

#include "Timer\Timer.h"

CImage::CImage( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution )
  : CGuiElement( aNode, screenResolution )
  , m_sActiveTexture( aNode.GetAttribute<std::string>( "default", "" ) )
  , m_bAnimated( aNode.GetAttribute<bool>( "is_animated", false ) )
  , m_bLoop( aNode.GetAttribute<bool>( "is_loop", false ) )
  , m_fTimePerImage( aNode.GetAttribute<float>( "time_per_image", 0.0f ) )
  , m_fCounter( 0.f )
  , m_Color( aNode.GetAttribute<Math::CColor>( "color", Math::CColor() ) )
  , m_eFlip( CGraphicsManager::NONE_FLIP )
  , m_bIsQuadrant( aNode.GetAttribute<bool>( "is_quadrant", false ) )
  , m_bIsBackGround( aNode.GetAttribute<bool>( "is_background", false ) )
  , mUseTextureSize ( aNode.GetAttribute<bool>( "use_texture_size", false ) )
  , mCurrentTextureId( 0 )
{
  const std::string& flip = aNode.GetAttribute<std::string>( "flip", "" );

  if ( flip == "FLIP_X" ) m_eFlip = CGraphicsManager::FLIP_X;
  else if ( flip == "FLIP_X" ) m_eFlip = CGraphicsManager::FLIP_Y;
  else m_eFlip = CGraphicsManager::NONE_FLIP;

  for ( int j = 0, count = aNode.GetNumChildren(); j < count; ++j )
  {
    const CXMLTreeNode& lTextureNode = aNode( j );
    const std::string& tagName = lTextureNode.GetName();
    if ( tagName == "texture" )
    {
      AddTexture( lTextureNode );
    }
  }
}

//---------------CGuiElement Interface----------------------
void CImage::Render()
{
  if ( GetVisible() )
  {
    CTexture* texture = ( m_bAnimated ) ? m_Textures.GetResourceById( mCurrentTextureId ) : m_Textures.GetResourceById( 0 );

    if ( texture )
    {
      uint32 w = 0;
      uint32 h = 0;
      
      if( mUseTextureSize )
      {
        w = texture->GetWidth();
        h = texture->GetHeight();
      }
      else
      {
        w = CGuiElement::m_uWidth;
        h = CGuiElement::m_uHeight;
      }

      if ( m_bIsQuadrant )
        h = CGuiElement::m_uWidth;

      if ( texture )
        GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, w, h, CGraphicsManager::CENTER, texture, m_eFlip );
      else
        GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, w, h, CGraphicsManager::CENTER, m_Color );
    }

    CGuiElement::RenderText();
  }

}

void CImage::Update()
{
  if ( m_bAnimated )
  {
    m_fCounter += deltaTimeMacro;

    if ( m_fCounter > m_fTimePerImage )
    {
      m_fCounter = 0.f;

      ++mCurrentTextureId;

      if( mCurrentTextureId >= m_Textures.GetResourcesCount() )
      {
        if( m_bLoop )
        {
          mCurrentTextureId = 0;
        }
        else
        {
          mCurrentTextureId = m_Textures.GetResourcesCount() - 1;
          m_bAnimated = false;
        }
      }
    }
  }
}

//---------------CImage Interface----------------------
void CImage::PlayAnimation( float timePerImage, bool loop )
{
  m_bAnimated        = true;
  m_bLoop            = loop;
  m_fTimePerImage    = timePerImage;
  mCurrentTextureId  = 0;
  m_fCounter         = 0.f;
}

void CImage::AddTexture( const CXMLTreeNode& aNode )
{
  CTexture* lTexture = aNode.GetAttribute<CTexture>("texture");
  if( lTexture )
  {
    bool lAdded        = m_Textures.AddResource( lTexture->GetName(), lTexture );
    ASSERT( lAdded, "The texture %s could not be added");
  }
}

void CImage::SetCurrentTextureIdx( uint32 aIdx )
{
  mCurrentTextureId = aIdx;
}
