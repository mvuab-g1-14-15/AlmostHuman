#include "Window/Windows.h"
#include "InputManager.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "GraphicsManager.h"
#include "Logger\Logger.h"
#include "ScriptManager.h"
#include "EngineManagers.h"
#include "xml/XMLTreeNode.h"
#include "Utils\ObjectFactory.h"

//---Elementos que puede contener una Windows--
#include "Widgets\Button.h"
#include "Widgets\CheckButton.h"
#include "Widgets\Slider.h"
#include "Widgets\EditableTextBox.h"
#include "Widgets\DialogBox.h"
#include "Widgets\RadioBox.h"
#include "Widgets\Image.h"
#include "Widgets\StaticText.h"
#include "Widgets\ProgressBar.h"
#include "Widgets\Map.h"
#include "Widgets\TextBox.h"
//---------------------------------------------

CWindows::~CWindows()
{
    std::vector<CGuiElement*>::iterator it( m_GuiElementsVector.begin() );
    std::vector<CGuiElement*>::iterator itEnd( m_GuiElementsVector.end() );

    while ( it != itEnd )
    {
        CHECKED_DELETE( *it );
        it++;
    }

    m_GuiElementsVector.clear();
}

bool CWindows::AddGuiElement( CGuiElement* inGuiElement )
{
    //Primero comprobamos que no exista ya el guiElement a introducir
    std::vector<CGuiElement*>::iterator it( m_GuiElementsVector.begin() );
    std::vector<CGuiElement*>::iterator itEnd( m_GuiElementsVector.end() );

    while ( it != itEnd )
    {
        CGuiElement* guiElement = *it;

        if ( guiElement == inGuiElement )
        {
            LOG_ERROR_APPLICATION( "Windows:: El GuiElement %s ya se ha introducido en la windows %s",
                                   inGuiElement->GetName().c_str(), m_sWindowsName.c_str() );
            return false;
        }

        it++;
    }

    //Finalmente lo insertamos en el vector de GuiElements
    m_GuiElementsVector.push_back( inGuiElement );
    return true;
}

bool CWindows::ReleaseGuiElement( const std::string& name )
{
    std::vector<CGuiElement*>::iterator it( m_GuiElementsVector.begin() );
    std::vector<CGuiElement*>::iterator itEnd( m_GuiElementsVector.end() );

    while ( it != itEnd )
    {
        if ( ( *it )->GetName().compare( name ) == 0 )
        {
            m_GuiElementsVector.erase( it );
            return true;
        }

        it++;
    }

    LOG_WARNING_APPLICATION( "Windows:: Se ha intentado eliminar el GuiElement %s pero no existe en la windows %s",
                             name.c_str(),
                             m_sWindowsName.c_str() );
    return false;
}

void CWindows::Render()
{
    std::vector<CGuiElement*>::iterator it( m_GuiElementsVector.begin() );
    std::vector<CGuiElement*>::iterator itEnd( m_GuiElementsVector.end() );

    while ( it != itEnd )
    {
        ( *it )->Render();
        it++;
    }
}


void CWindows::Update()
{

    if ( m_sLuaCode_OnUpdateWindows.compare( "" ) )
    {
        //Lanzar acción en Lua:
        //CScriptManager* scriptManager = CCORE->GetScriptManager();
        ScriptMInstance->RunCode( m_sLuaCode_OnUpdateWindows );
    }

    std::vector<CGuiElement*>::reverse_iterator revIt;
    std::vector<CGuiElement*>::reverse_iterator revItEnd( m_GuiElementsVector.rend() );
    std::vector<CGuiElement*>::reverse_iterator revIt_aux;
    bool focus_aux = false;

    //Primero de todo quitamos el focus a todos los GuiElements
    for ( revIt = m_GuiElementsVector.rbegin(); revIt != revItEnd; ++revIt )
    {
        // De haber alguno que tenga focus nos lo guardamos, ya que al recalcular el focus si no lo tiene nadie
        // como mínimo se lo damos al que lo tenía.
        if ( ( *revIt )->HasFocus() )
        {
            focus_aux = true;
            revIt_aux = revIt;
        }

        ( *revIt )->LoseFocus();
    }

    bool elemWithFocus = false;

    for ( revIt = m_GuiElementsVector.rbegin(); revIt != revItEnd; ++revIt )
    {
        ( *revIt )->Update();

        if ( ( *revIt )->HasFocus() )
        {
            elemWithFocus = true;
            break;
        }
    }

    if ( elemWithFocus )
    {
        CGuiElement* guiElement = ( *revIt );
        m_GuiElementsVector.erase( revIt.base() - 1 );
        m_GuiElementsVector.push_back( guiElement );
    }
    else if ( focus_aux )
    {
        ( *revIt_aux )->GainFocus();
        ( *revIt_aux )->Update();
    }

    IsKeyDown();
}


void CWindows::RegisterElements( std::map<std::string, CGuiElement*>& elements )
{
    std::map<std::string, CGuiElement*>::const_iterator it_aux;

    std::vector<CGuiElement*>::iterator it( m_GuiElementsVector.begin() );
    std::vector<CGuiElement*>::iterator itEnd( m_GuiElementsVector.end() );

    while ( it != itEnd )
    {
        CGuiElement* element = ( *it );

        //Antes de insertarlo en el mapa debemos comprobar que no este registrado ya un GuiElement con el mismo identificado(mismo string):
        if ( elements.find( element->GetName() ) != elements.end() )
        {
            LOG_ERROR_APPLICATION( "Windows:: El GuiElement %s ya se ha registrado en el GuiManager", element->GetName().c_str() );
        }
        else
        {
            elements.insert( std::pair<std::string, CGuiElement*>( element->GetName(), element ) );
        }

        it++;
    }
}

void CWindows::LoadWindows( void )
{
    std::vector<CGuiElement*>::iterator it;
    std::vector<CGuiElement*>::iterator itEnd( m_GuiElementsVector.end() );

    for ( it = m_GuiElementsVector.begin(); it != itEnd; ++it )
    {
        CGuiElement* guiElement = ( *it );
        guiElement->OnLoadValue();
    }

    if ( m_sLuaCode_OnLoadWindows.compare( "" ) )
    {
        //Lanzar acción en Lua:
        //CScriptManager* scriptManager = CCORE->GetScriptManager();
        ScriptMInstance->RunCode( m_sLuaCode_OnLoadWindows );
    }

}

void CWindows::SaveWindows( void )
{

    std::vector<CGuiElement*>::iterator it;
    std::vector<CGuiElement*>::iterator itEnd( m_GuiElementsVector.end() );

    for ( it = m_GuiElementsVector.begin(); it != itEnd; ++it )
    {
        CGuiElement* guiElement = ( *it );
        guiElement->OnSaveValue();
    }

    if ( m_sLuaCode_OnSaveWindows.compare( "" ) )
    {
        //Lanzar acción en Lua:
        //CScriptManager* scriptManager = CCORE->GetScriptManager();
        ScriptMInstance->RunCode( m_sLuaCode_OnSaveWindows );
    }
}

void CWindows::IsKeyDown()
{
    if ( m_sLuaCode_OnKeyDown.compare( "" ) )
    {
        //Comprobar si el input se cumple
        if ( InputManagerInstance->IsUpDown( IDV_KEYBOARD, m_uInputKeyDown ) )
        {
            //Lanzar acción en Lua:
            //CScriptManager* scriptManager = CCORE->GetScriptManager();
            ScriptMInstance->RunCode( m_sLuaCode_OnKeyDown );
        }
    }
}

bool CWindows::LoadXML( const std::string& xmlGuiFile, const Math::Vect2i& screenResolution )
{
    ObjectFactory2<CGuiElement, const CXMLTreeNode&, const Math::Vect2i&, std::string > ManagerFactory;

    ManagerFactory.Register( "Button", Type2Type<CButton>( ) );
    ManagerFactory.Register( "CheckButton", Type2Type<CCheckButton>( ) );
    ManagerFactory.Register( "Slider", Type2Type<CSlider>( ) );
    ManagerFactory.Register( "DialogBox", Type2Type<CDialogBox>( ) );
    ManagerFactory.Register( "EditableTextBox", Type2Type<CEditableTextBox>( ) );
    ManagerFactory.Register( "RadioBox", Type2Type<CRadioBox>( ) );
    ManagerFactory.Register( "Image", Type2Type<CImage>( ) );
    ManagerFactory.Register( "ProgressBar", Type2Type<CProgressBar>( ) );
    ManagerFactory.Register( "StaticText", Type2Type<CStaticText>( ) );
    ManagerFactory.Register( "Map", Type2Type<CMap>( ) );
    ManagerFactory.Register( "TextBox", Type2Type<CTextBox>( ) );

    CXMLTreeNode l_File, lWindows;

    if ( l_File.LoadAndFindNode( xmlGuiFile.c_str(), "Windows", lWindows ) )
    {
        m_sLuaCode_OnLoadWindows    = lWindows.GetAttribute<std::string>( "OnLoadWindows", "" );
        m_sLuaCode_OnSaveWindows    = lWindows.GetAttribute<std::string>( "OnSaveWindows", "" );
        m_sLuaCode_OnUpdateWindows  = lWindows.GetAttribute<std::string>( "OnUpdateWindows", "" );
        m_sLuaCode_OnKeyDown        = lWindows.GetAttribute<std::string>( "OnKeyDown", "" );
        m_uInputKeyDown             = lWindows.GetAttribute<int32>( "input", 0 );

        for ( int i = 0, count = lWindows.GetNumChildren(); i < count; ++i )
        {
            const CXMLTreeNode& Node = lWindows( i );

            const std::string& TagName = Node.GetName();

            CGuiElement* lGuiElement = ManagerFactory.Create( TagName.c_str(), Node, screenResolution);

            if ( !lGuiElement )
            {
                LOG_ERROR_APPLICATION( "Gui element not found %s!", TagName.c_str() );
            }
            else
            {
                if ( !AddGuiElement( lGuiElement ) )
                {
                    CHECKED_DELETE( lGuiElement );
                }
            }
        }
    }
    else
    {
        LOG_ERROR_APPLICATION( "Could not open window node in file %s", xmlGuiFile.c_str() );
    }

    return true;
}

CGuiElement* CWindows::GetElement( const std::string& NameElement )
{
    std::vector<CGuiElement*>::iterator it( m_GuiElementsVector.begin() );
    std::vector<CGuiElement*>::iterator itEnd( m_GuiElementsVector.end() );

    while ( it != itEnd )
    {
        if ( ( *it )->GetName() == NameElement )
        {
            return ( *it );
        }

        ++it;
    }

    return 0;
}