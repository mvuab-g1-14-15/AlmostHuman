
#include "Widgets\ConsoleGUI.h"
#include "Widgets\EditableTextBox.h"
#include "ActionManager.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "Fonts/FontManager.h"
#include "EngineManagers.h"
#include "ScriptManager.h"
#include "GUIManager.h"

#include "Timer\Timer.h"

//---Constructor
CConsoleGUI::CConsoleGUI( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution )
    : CEditableTextBox( aNode, screenResolution )
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
        {
            SetVisible( false );
            GUIInstance->SetRenderPointer(false);
        }
        else
        {
            SetVisible( true );
            GUIInstance->SetRenderPointer(true);
        }
    }

    CEditableTextBox::Update();

    if ( IsReturnPress() )
    {
        ScriptMInstance->RunCode( GetBuffer() );
        std::vector<std::string>::iterator it = m_vBuffer.begin()
                                                , it_end = m_vBuffer.end();

        for ( ; it != it_end; ++it )
            if ( *it == GetBuffer() )
            {
                break;
            }

        if ( it == it_end )
        {
            m_vBuffer.push_back( GetBuffer() );
            m_itActual = m_vBuffer.end();
            --m_itActual;
        }

        /*  m_vBuffer.push_back( GetBuffer() );
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




