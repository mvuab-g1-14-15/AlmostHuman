

#include "StaticText.h"
#include "GraphicsManager.h"
#include "Logger/Logger.h"
#include "EngineManagers.h"
#include "Timer/Timer.h"

CStaticText::CStaticText( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution )
    : CGuiElement( aNode, screenResolution )
    , mAnimateText( aNode.GetAttribute<bool>("animate_font", false))
    , mTime( 0.0f )
    , mTimeVisible( aNode.GetAttribute<float>("time_visible", 0.0f) )
    , mTimeNoVisible( aNode.GetAttribute<float>("time_no_visible", 0.0f) )
{
}

//---------------Interfaz de GuiElement----------------------
void    CStaticText::Render ()
{
    if( IsVisible() )
    {
        //Primero renderizamos todos los hijos que pudiera tener el Static Text:
        CGuiElement::Render();

        //Finalmente renderizamos el texto:
        CGuiElement::RenderText();
    }
}

void CStaticText::Update()
{
  if( mAnimateText )
  {
    mTime += constFrameTime;
    if( mTime > mTimeVisible && IsVisible() )
    {
      SetVisible( false );
      mTime = 0.0f;
    }
    
    if( mTime > mTimeNoVisible && !IsVisible() )
    {
      SetVisible( true );
      mTime = 0.0f;
    }
  }

  CGuiElement::Update();
}

void CStaticText::SetLiteral( const std::string& lit)
{
    CGuiElement::m_sLiteral = lit;
}