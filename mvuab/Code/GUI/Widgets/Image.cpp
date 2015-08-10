

#include "Widgets\Image.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

#include "Timer\Timer.h"

CImage::CImage( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution )
  : CGuiElement( aNode, screenResolution )
  , m_sActiveTexture( aNode.GetAttribute<std::string>( "default", "" ) )
  , m_bAnimated( false )
  , m_bLoop( false )
  , m_fTimePerImage( 0.f )
  , m_fCounter( 0.f )
  , m_Color( aNode.GetAttribute<Math::CColor>( "color", Math::CColor() ) )
  , m_eFlip( CGraphicsManager::NONE_FLIP )
  , m_bIsQuadrant( aNode.GetAttribute<bool>( "is_quadrant", false ) )
  , m_bIsBackGround( aNode.GetAttribute<bool>( "is_background", false ) )
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
      SetTexture( lTextureNode.GetAttribute<CTexture>( "name_texture" ), lTextureNode.GetAttribute<std::string>( "name", "" ) );
  }
}

//---------------CGuiElement Interface----------------------
void CImage::Render()
{
  if ( CGuiElement::m_bIsVisible )
  {
    CTexture* texture = NULL;

    if ( m_bAnimated )
      texture = *m_itVecTextures;
    else
    {
      tTexturesMap::iterator it = m_Textures.find( m_sActiveTexture );

      if ( it != m_Textures.end() )
        texture = it->second;
    }

    if ( texture )
    {
      uint32 w = CGuiElement::m_uWidth;
      uint32 h = CGuiElement::m_uHeight;

      if ( m_bIsQuadrant )
        h = CGuiElement::m_uWidth;

      //TODO RAUL
      if ( texture )
        GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, w, h, CGraphicsManager::UPPER_LEFT, texture, m_eFlip );
      else
        GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, w, h, CGraphicsManager::UPPER_LEFT, m_Color );
    }

    //Finalmente renderizamos el texto:
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
      m_itVecTextures++;

      if ( m_itVecTextures == m_VecTextures.end() )
      {
        m_itVecTextures = m_VecTextures.begin();

        if ( !m_bLoop )
          m_bAnimated = false;
      }

      m_fCounter = 0.f;
    }
  }
}

//---------------CImage Interface----------------------
void CImage::PlayAnimation( float timePerImage, bool loop )
{
  assert( m_Textures.size() > 0 );

  m_bAnimated = true;
  m_bLoop  = loop;
  m_fTimePerImage = timePerImage;
  m_itVecTextures = m_VecTextures.begin();
  m_fCounter  = 0.f;
}

void CImage::SetTexture( CTexture* texture, std::string name )
{
  m_Textures.insert( std::pair<std::string, CTexture*>( name, texture ) );
  m_VecTextures.push_back( texture );
}


