
#include "Widgets\ConsoleGUI.h"
#include "Widgets\EditableTextBox.h"
#include "ActionManager.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "Fonts/FontManager.h"
#include "EngineManagers.h"
#include "ScriptManager.h"

#include "Timer\Timer.h"

//---Constructor
CConsoleGUI::CConsoleGUI( uint32 windowsHeight, uint32 windowsWidth, float height_precent,
                          float witdh_percent,
                          const Math::Vect2f position_percent, Math::CColor textColor, uint32 fontID,
                          std::string lit, uint32 textHeightOffset, uint32 textWidthOffset,
                          bool isVisible,  bool isActive )
  : CEditableTextBox( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, textColor, fontID, lit,
                      textHeightOffset, textWidthOffset, isVisible, isActive )

{
}

CConsoleGUI::~CConsoleGUI()
{
  CEditableTextBox::~CEditableTextBox();
}

//---------------Interfaz de GuiElement----------------------
void CConsoleGUI::Render()
{
  CEditableTextBox::Render();
}

void CConsoleGUI::Update()
{
  if ( ActionManagerInstance->DoAction( "ConsolaGUI" ) )
  {
    if ( GetVisible() )
      SetVisible( false );
    else
      SetVisible( true );
  }

  CEditableTextBox::Update();

  if ( IsReturnPress() )
  {
    ScriptMInstance->RunCode( GetBuffer() );
    std::vector<std::string>::iterator it = m_vBuffer.begin()
                                            , it_end = m_vBuffer.end();

    for ( ; it != it_end; ++it )
      if ( *it == GetBuffer() )
        break;

    if ( it == it_end )
    {
      m_vBuffer.push_back( GetBuffer() );
      m_itActual = m_vBuffer.end();
      --m_itActual;
    }

    /*m_vBuffer.push_back( GetBuffer() );
    m_itActual = m_vBuffer.end();
    --m_itActual;*/
    SetBuffer( "" );
    SetsFocusObject( " " );
    SetbShift( false );
    SetuCursorPos( 0 );
  }

  if ( ActionManagerInstance->DoAction( "ForwardGUI" ) )
  {

    if ( m_itActual == m_vBuffer.begin() )
    {
      SetBuffer( ( *m_itActual ) );
      m_itActual = m_vBuffer.end();
      --m_itActual;
    }
    else if ( m_itActual == m_vBuffer.end() )
    {
      --m_itActual;
      SetBuffer( ( *m_itActual ) );
      --m_itActual;
    }
    else
    {
      SetBuffer( ( *m_itActual ) );
      --m_itActual;
    }
  }
  else if ( ActionManagerInstance->DoAction( "BackwarGUI" ) )
  {

    if ( m_itActual == m_vBuffer.end() )
    {
      m_itActual = m_vBuffer.begin();
      SetBuffer( ( *m_itActual ) );
      ++m_itActual;
    }
    else
    {
      SetBuffer( ( *m_itActual ) );
      ++m_itActual;
    }
  }
}




