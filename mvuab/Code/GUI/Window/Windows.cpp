#include "Window/Windows.h"
#include "InputManager.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "GraphicsManager.h"
#include "Logger\Logger.h"
#include "ScriptManager.h"
#include "EngineManagers.h"
#include "xml/XMLTreeNode.h"

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
    //Read the xml gui file
    LOG_INFO_APPLICATION( "CWindows:: Iniciando el parseo del fichero %s", xmlGuiFile.c_str() );
    bool isOK = false;

    CXMLTreeNode newFile;

    if ( !newFile.LoadFile( xmlGuiFile.c_str() ) )
    {
        LOG_ERROR_APPLICATION( "Windows:: No se ha podido leer correctamente el fichero ->%s", xmlGuiFile.c_str() );
        isOK = false;
    }
    else
    {
        CTextureManager* textureM = TextureMInstance;
        CXMLTreeNode windows = newFile["Windows"];

        m_sLuaCode_OnLoadWindows  = windows.GetPszProperty( "OnLoadWindows" );
        m_sLuaCode_OnSaveWindows  = windows.GetPszProperty( "OnSaveWindows" );
        m_sLuaCode_OnUpdateWindows = windows.GetPszProperty( "OnUpdateWindows" );


        if ( windows.Exists() )
        {
            int count = windows.GetNumChildren();

            for ( int i = 0; i < count; ++i )
            {
                CXMLTreeNode pNewNode = windows( i );

                //Para cada guielement leemos su informacion comun-->
                std::string name = pNewNode.GetPszProperty( "name", "defaultGuiElement" );
                float posx = pNewNode.GetFloatProperty( "posx", 0.f );
                float posy = pNewNode.GetFloatProperty( "posy", 0.f );
                float w  = pNewNode.GetFloatProperty( "width", 50.f );
                float h  = pNewNode.GetFloatProperty( "height", 50.f );
                bool visible = pNewNode.GetBoolProperty( "visible", true );
                bool activated  = pNewNode.GetBoolProperty( "active", true );

                std::string tagName = pNewNode.GetName();

                if ( tagName.compare( "Button" ) == 0 )
                {
                    CButton* newButton = NULL;
                    newButton = LoadButton( pNewNode, screenResolution, textureM );
                    AddGuiElement( newButton );
                }
                else if ( tagName.compare( "CheckButton" ) == 0 )
                {
                    CCheckButton* new_checkButton = NULL;
                    new_checkButton = LoadCheckButton( pNewNode, screenResolution, textureM );
                    AddGuiElement( new_checkButton );
                }
                else if ( tagName.compare( "Slider" ) == 0 )
                {
                    CSlider* new_slider = NULL;
                    new_slider = LoadSlider( pNewNode, screenResolution, textureM );
                    AddGuiElement( new_slider );
                }
                else if ( tagName.compare( "DialogBox" ) == 0 )
                {
                    CDialogBox* new_dialogBox = NULL;
                    new_dialogBox = LoadDialogBox( pNewNode, screenResolution, textureM );
                    AddGuiElement( new_dialogBox );
                }
                else if ( tagName.compare( "EditableTextBox" ) == 0 )
                {
                    CEditableTextBox* new_editableTextBox = NULL;
                    new_editableTextBox = LoadEditableTextBox( pNewNode, screenResolution, textureM );
                    AddGuiElement( new_editableTextBox );
                }
                else if ( tagName.compare( "RadioBox" ) == 0 )
                {
                    CRadioBox* new_radioBox = NULL;
                    new_radioBox = LoadRadioBox( pNewNode, screenResolution, textureM );
                    AddGuiElement( new_radioBox );
                }
                else if ( tagName.compare( "Image" ) == 0 )
                {
                    CImage* new_image = NULL;
                    new_image = _LoadImage( pNewNode, screenResolution, textureM );
                    AddGuiElement( new_image );
                }
                else if ( tagName.compare( "ProgressBar" ) == 0 )
                {
                    CProgressBar* new_progressBar = NULL;
                    new_progressBar = LoadProgressBar( pNewNode, screenResolution, textureM );
                    new_progressBar->SetProgress( 0.0f );
                    AddGuiElement( new_progressBar );
                }
                else if ( tagName.compare( "StaticText" ) == 0 )
                {
                    CStaticText* new_staticText = NULL;
                    new_staticText = LoadStaticText( pNewNode, screenResolution, textureM );
                    AddGuiElement( new_staticText );
                }
                else if ( tagName.compare( "Map" ) == 0 )
                {
                    CMap* new_Map = NULL;
                    new_Map = LoadMap( pNewNode, screenResolution );
                    AddGuiElement( new_Map );
                }
                else if ( tagName.compare( "KeyBoard_Back" ) == 0 )
                {
                    //<KeyBoard_Back input="DIK_A" OnKeyDown="blablaLua"/>
                    m_sLuaCode_OnKeyDown = pNewNode.GetPszProperty( "OnKeyDown", "" );
                    m_uInputKeyDown  = pNewNode.GetAttribute<int32>( "input", 0 );
                }
                else
                {
                    //Warning
                    LOG_WARNING_APPLICATION( "Windows:: No se reconoce el tag %s del fichero %s", tagName.c_str(), xmlGuiFile.c_str() );
                }

            }

            isOK = true;
        }
        else
        {
            LOG_ERROR_APPLICATION( "Windows:: No se ha podido leer el tag Windows del fichero ->%s", xmlGuiFile.c_str() );
            isOK = false;
        }

    }//END else de if (!newFile.LoadFile(xmlGuiFile.c_str()))


    if ( !isOK )
    {
        LOG_ERROR_APPLICATION( "Windows:: No se ha podido leer correctamente el fichero -->%s", xmlGuiFile.c_str() );
        isOK = false;
    }
    else
    {
        LOG_INFO_APPLICATION( "CWindows:: Finalizado correctamente el parseo el fichero %s", xmlGuiFile.c_str() );
    }


    return isOK;
}


CButton* CWindows::LoadButton( CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution, CTextureManager* tm )
{
    //<Button name="play" posx="0" posy="0" height="10" width="10" visible="true" activated="true"
    // texture_normal="blabla" texture_over="bla" texture_clicked="bla" texture_deactivated="bla"
    // OnClickedAction="blabla" OnOverAction="blabla" Literal="blabla" widthOffset="" heightOffset=""/>

    std::string name = pNewNode.GetPszProperty( "name", "defaultGuiElement" );
    float  posx = pNewNode.GetFloatProperty( "posx", 0.f );
    float  posy = pNewNode.GetFloatProperty( "posy", 0.f );
    float  w = pNewNode.GetFloatProperty( "width", 50.f );
    float  h = pNewNode.GetFloatProperty( "height", 50.f );
    bool visible = pNewNode.GetBoolProperty( "visible", true );
    bool activated  = pNewNode.GetBoolProperty( "active", true );
    std::string texture_normal = pNewNode.GetPszProperty( "texture_normal", "" );
    std::string texture_over = pNewNode.GetPszProperty( "texture_over", "" );
    std::string texture_clicked = pNewNode.GetPszProperty( "texture_clicked", "" );
    std::string texture_deactivated = pNewNode.GetPszProperty( "texture_deactivated", "" );
    std::string OnClickedAction = pNewNode.GetPszProperty( "OnClickedAction", "" );
    std::string OnOverAction = pNewNode.GetPszProperty( "OnOverAction", "" );
    std::string l_literal  = pNewNode.GetPszProperty( "Literal", "" );
    float  widthOffsetPercent = pNewNode.GetFloatProperty( "widthOffset", 0.f );
    float  heightOffsetPercent = pNewNode.GetFloatProperty( "heightOffset", 0.f );

    CTexture* normal = tm->GetTexture( texture_normal );
    CTexture* over = tm->GetTexture( texture_over );
    CTexture* clicked = tm->GetTexture( texture_clicked );
    CTexture* deactivated  = tm->GetTexture( texture_deactivated );

    uint32 widthOffset = ( uint32 )( screenResolution.x * 0.01f * widthOffsetPercent );
    uint32 heightOffset = ( uint32 )( screenResolution.y * 0.01f * heightOffsetPercent );

    CButton* button = new CButton( screenResolution.y, screenResolution.x, h, w, Math::Vect2f( posx, posy ), l_literal,
                                   heightOffset, widthOffset,
                                   visible, activated );
    button->SetName( name );
    button->SetTextures( normal, over, clicked, deactivated );
    button->SetOnClickedAction( OnClickedAction );
    button->SetOnOverAction( OnOverAction );
    return button;
}

CCheckButton* CWindows::LoadCheckButton( CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution,
        CTextureManager* tm )
{
    //<CheckButton name="debug_thps" posx="0" posy="2" height="10" width="10" visible="true" active="true" isOn="true"
    // texture_on="blabla" texture_off="bla" texture_deactivated"bla"
    // OnCheckOn="blabla" OnCheckOff="blabla" OnOverButton="blabla"
    // OnSaveValue="blabl" OnLoadValue="" Literal="blabla" widthOffset="" heightOffset=""/>

    std::string name  = pNewNode.GetPszProperty( "name", "defaultGuiElement" );
    float  posx  = pNewNode.GetFloatProperty( "posx", 0.f );
    float  posy  = pNewNode.GetFloatProperty( "posy", 0.f );
    float  w  = pNewNode.GetFloatProperty( "width", 50.f );
    float  h  = pNewNode.GetFloatProperty( "height", 50.f );
    bool visible  = pNewNode.GetBoolProperty( "visible", true );
    bool activated = pNewNode.GetBoolProperty( "active", true );
    bool isOn  = pNewNode.GetBoolProperty( "isOn", "" );
    std::string texture_on = pNewNode.GetPszProperty( "texture_on", "" );
    std::string texture_off  = pNewNode.GetPszProperty( "texture_off", "" );
    std::string texture_deactivated  = pNewNode.GetPszProperty( "texture_deactivated", "" );
    std::string OnCheckOn = pNewNode.GetPszProperty( "OnCheckOn", "" );
    std::string OnCheckOff = pNewNode.GetPszProperty( "OnCheckOff", "" );
    std::string OnOverButton  = pNewNode.GetPszProperty( "OnOverButton", "" );
    std::string OnSaveValue  = pNewNode.GetPszProperty( "OnSaveValue", "" );
    std::string OnLoadValue  = pNewNode.GetPszProperty( "OnLoadValue", "" );
    std::string l_literal = pNewNode.GetPszProperty( "Literal", "" );
    float  widthOffsetPercent  = pNewNode.GetFloatProperty( "widthOffset", 0.f );
    float  heightOffsetPercent  = pNewNode.GetFloatProperty( "heightOffset", 0.f );


    CTexture* on  = tm->GetTexture( texture_on );
    CTexture* off = tm->GetTexture( texture_off );
    CTexture* deactivated = tm->GetTexture( texture_deactivated );

    uint32 widthOffset = ( uint32 )( screenResolution.x * 0.01f * widthOffsetPercent );
    uint32 heightOffset = ( uint32 )( screenResolution.y * 0.01f * heightOffsetPercent );

    CCheckButton* checkButton = new CCheckButton( screenResolution.y, screenResolution.x, h, w, Math::Vect2f( posx, posy ),
            isOn, l_literal, heightOffset,
            widthOffset, visible, activated );
    checkButton->SetName( name );
    checkButton->SetTextures( on, off, deactivated );
    checkButton->SetOnCheckOnAction( OnCheckOn );
    checkButton->SetOnCheckOffAction( OnCheckOff );
    checkButton->SetOnLoadValueAction( OnLoadValue );
    checkButton->SetOnSaveValueAction( OnSaveValue );
    checkButton->SetOnOverAction( OnOverButton );
    return checkButton;
}

CSlider* CWindows::LoadSlider( CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution, CTextureManager* tm )
{
    //<Slider name="debug_fps" posx="0" posy="2" height="10" width="10" visible="true" active="true" value="0.5" buttonH="10" buttonW="10"
    // button_normal="blabla" button_over="bla" button_clicked"bla" button_deactivated="bla" quad="bla"
    // OnOverAction="" OnClickedAction="" OnChangeValue="" OnSaveValue="blabl" OnLoadValue="" Literal="blabla" widthOffset="" heightOffset=""/>

    std::string name = pNewNode.GetPszProperty( "name", "defaultGuiElement" );
    float  posx = pNewNode.GetFloatProperty( "posx", 0.f );
    float  posy = pNewNode.GetFloatProperty( "posy", 0.f );
    float  w = pNewNode.GetFloatProperty( "width", 50.f );
    float  h = pNewNode.GetFloatProperty( "height", 50.f );
    bool visible = pNewNode.GetBoolProperty( "visible", true );
    bool activated  = pNewNode.GetBoolProperty( "active", true );
    float  value  = pNewNode.GetBoolProperty( "value", 0.f );
    float  buttonH = pNewNode.GetFloatProperty( "buttonH", 10.f );
    float  buttonW = pNewNode.GetFloatProperty( "buttonW", 10.f );
    std::string button_normal  = pNewNode.GetPszProperty( "button_normal", "" );
    std::string button_over = pNewNode.GetPszProperty( "button_over", "" );
    std::string button_clicked = pNewNode.GetPszProperty( "button_clicked", "" );
    std::string button_deactivated = pNewNode.GetPszProperty( "button_deactivated", "" );
    std::string quad = pNewNode.GetPszProperty( "quad", "" );
    std::string OnChangeValue  = pNewNode.GetPszProperty( "OnChangeValue", "" );
    std::string OnClickedAction = pNewNode.GetPszProperty( "OnClickedAction", "" );
    std::string OnOverAction = pNewNode.GetPszProperty( "OnOverAction", "" );
    std::string OnSaveValue = pNewNode.GetPszProperty( "OnSaveValue", "" );
    std::string OnLoadValue = pNewNode.GetPszProperty( "OnLoadValue", "" );
    std::string l_literal  = pNewNode.GetPszProperty( "Literal", "" );
    float  widthOffsetPercent = pNewNode.GetFloatProperty( "widthOffset", 0.f );
    float  heightOffsetPercent = pNewNode.GetFloatProperty( "heightOffset", 0.f );

    CTexture* normal = tm->GetTexture( button_normal ); //GetTexture(button_normal);
    CTexture* over  = tm->GetTexture( button_over );
    CTexture* deac  = tm->GetTexture( button_deactivated );
    CTexture* clicked = tm->GetTexture( button_clicked );
    CTexture* back  = tm->GetTexture( quad );

    uint32 widthOffset = ( uint32 )( screenResolution.x * 0.01f * widthOffsetPercent );
    uint32 heightOffset = ( uint32 )( screenResolution.y * 0.01f * heightOffsetPercent );

    CSlider* slider = new CSlider( screenResolution.y, screenResolution.x, h, w, Math::Vect2f( posx, posy ), buttonW,
                                   buttonH, value,
                                   l_literal, heightOffset, widthOffset, visible, activated );
    slider->SetName( name );
    slider->SetButtonTextures( normal, over, clicked, deac );
    slider->SetBackGroundTexture( back );
    slider->SetOnChangeValueAction( OnChangeValue );
    slider->SetOnClickedAction( OnClickedAction );
    slider->SetOnOverAction( OnOverAction );
    slider->SetOnLoadValueAction( OnLoadValue );
    slider->SetOnSaveValueAction( OnSaveValue );
    return slider;
}

CDialogBox* CWindows::LoadDialogBox( CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution, CTextureManager* tm )
{
    //<DialogBox name="debug_fps" posx="0" posy="2" height="10" width="10" visible="true" active="true" buttonH="10" buttonW="10"
    // buttonClose_normal="blabla" buttonClose_over="bla" buttonClose_clicked"bla" buttonClose_deactivated"bla" buttonMove_normal="blabla" buttonMove_over="bla"
    // buttonMove_clicked"bla" buttonMove_deactivated"bla" quad="bla" Literal="blabla" widthOffset="" heightOffset=""/>

    const std::string& name  = pNewNode.GetPszProperty( "name", "defaultGuiElement" );
    float posx  = pNewNode.GetFloatProperty( "posx", 0.f );
    float posy  = pNewNode.GetFloatProperty( "posy", 0.f );
    float w  = pNewNode.GetFloatProperty( "width", 50.f );
    float h  = pNewNode.GetFloatProperty( "height", 50.f );
    bool visible = pNewNode.GetBoolProperty( "visible", true );
    bool activated = pNewNode.GetBoolProperty( "active", true );
    float buttonH  = pNewNode.GetFloatProperty( "buttonH", 10.f );
    float buttonW  = pNewNode.GetFloatProperty( "buttonW", 10.f );
    const std::string& buttonClose_normal = pNewNode.GetPszProperty( "buttonClose_normal", "" );
    const std::string& buttonClose_over = pNewNode.GetPszProperty( "buttonClose_over", "" );
    const std::string& buttonClose_clicked = pNewNode.GetPszProperty( "buttonClose_clicked", "" );
    const std::string& buttonClose_deactivated = pNewNode.GetPszProperty( "buttonClose_deactivated", "" );
    const std::string& buttonMove_normal  = pNewNode.GetPszProperty( "buttonMove_normal", "" );
    const std::string& buttonMove_over = pNewNode.GetPszProperty( "buttonMove_over", "" );
    const std::string& buttonMove_clicked = pNewNode.GetPszProperty( "buttonMove_clicked", "" );
    const std::string& buttonMove_deactivated = pNewNode.GetPszProperty( "buttonMove_deactivated", "" );
    const std::string& quad = pNewNode.GetPszProperty( "quad" );
    const std::string& l_literal  = pNewNode.GetPszProperty( "Literal", "" );
    float  widthOffsetPercent = pNewNode.GetFloatProperty( "widthOffset", 0.f );
    float  heightOffsetPercent = pNewNode.GetFloatProperty( "heightOffset", 0.f );

    CTexture* Close_normal = tm->GetTexture( buttonClose_normal ); //GetTexture(buttonClose_normal);
    CTexture* Close_over = tm->GetTexture( buttonClose_over );
    CTexture* Close_clicked = tm->GetTexture( buttonClose_clicked );
    CTexture* Close_deactivated = tm->GetTexture( buttonClose_deactivated );
    CTexture* Move_normal  = tm->GetTexture( buttonMove_normal );
    CTexture* Move_over = tm->GetTexture( buttonMove_over );
    CTexture* Move_clicked = tm->GetTexture( buttonMove_clicked );
    CTexture* Move_deactivated = tm->GetTexture( buttonMove_deactivated );
    CTexture* back  = tm->GetTexture( quad );

    uint32 widthOffset = ( uint32 )( screenResolution.x * 0.01f * widthOffsetPercent );
    uint32 heightOffset = ( uint32 )( screenResolution.y * 0.01f * heightOffsetPercent );

    CDialogBox* dialogBox = new CDialogBox( screenResolution.y, screenResolution.x, h, w, Math::Vect2f( posx, posy ),
                                            buttonW, buttonH, l_literal,
                                            heightOffset, widthOffset, visible, activated );
    dialogBox->SetName( name );
    dialogBox->SetCloseButtonTextures( Close_normal, Close_over, Close_clicked, Close_deactivated );
    dialogBox->SetMoveButtonTextures( Move_normal, Move_over, Move_clicked, Move_deactivated );
    dialogBox->SetDialogTexture( back );

    return dialogBox;
}

CEditableTextBox* CWindows::LoadEditableTextBox( CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution,
        CTextureManager* tm )
{
    //<EditableTextBox name="debug_fps" posx="0" posy="2" height="10" width="10" visible="true" active="true"
    // texture_quad="blabla" id_font="0" color_font_r="0" color_font_g="0" color_font_b="0"
    // buffer="" OnSaveValue="blabla" OnLoadValue="blabla" Literal="blabla" widthOffset="" heightOffset=""
    // Literal="blabla" widthOffset="" heightOffset=""/>

    std::string name = pNewNode.GetPszProperty( "name", "defaultGuiElement" );
    float  posx = pNewNode.GetFloatProperty( "posx", 0.f );
    float  posy = pNewNode.GetFloatProperty( "posy", 0.f );
    float  w = pNewNode.GetFloatProperty( "width", 50.f );
    float  h = pNewNode.GetFloatProperty( "height", 50.f );
    bool visible = pNewNode.GetBoolProperty( "visible", true );
    bool activated  = pNewNode.GetBoolProperty( "active", true );
    std::string quad = pNewNode.GetPszProperty( "texture_quad", "" );
    std::string OnSaveValue = pNewNode.GetPszProperty( "OnSaveValue", "" );
    std::string OnLoadValue = pNewNode.GetPszProperty( "OnLoadValue", "" );
    uint32 idFont  = pNewNode.GetAttribute<int32>( "id_font", 0 );
    float  color_font_r = pNewNode.GetFloatProperty( "color_font_r", 0.f );
    float  color_font_g = pNewNode.GetFloatProperty( "color_font_g", 0.f );
    float  color_font_b = pNewNode.GetFloatProperty( "color_font_b", 0.f );
    std::string buffer  = pNewNode.GetPszProperty( "buffer", "" );
    std::string l_literal  = pNewNode.GetPszProperty( "Literal", "" );
    float  widthOffsetPercent = pNewNode.GetFloatProperty( "widthOffset", 0.f );
    float  heightOffsetPercent = pNewNode.GetFloatProperty( "heightOffset", 0.f );

    uint32 widthOffset = ( uint32 )( screenResolution.x * 0.01f * widthOffsetPercent );
    uint32 heightOffset = ( uint32 )( screenResolution.y * 0.01f * heightOffsetPercent );

    CTexture* quad_texture = tm->GetTexture( quad ); //GetTexture(quad);

    CEditableTextBox* editableTextBox = new CEditableTextBox( screenResolution.y, screenResolution.x, h, w,
            Math::Vect2f( posx, posy ),
            Math::CColor( color_font_r, color_font_g, color_font_b ), idFont, l_literal,
            heightOffset, widthOffset, visible, activated );
    editableTextBox->SetName( name );
    editableTextBox->SetBackGroundTexture( quad_texture );
    editableTextBox->SetBuffer( buffer );
    editableTextBox->SetOnLoadValueAction( OnSaveValue );
    editableTextBox->SetOnSaveValueAction( OnLoadValue );

    return editableTextBox;
}


CRadioBox* CWindows::LoadRadioBox( CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution, CTextureManager* tm )
{
    //<RadioBox name="pepito" posx="0" posy="2" height="10" width="10" default_checkButton="bla" visible="true" active="true" texture_back=""
    // columns="3" rows="3" OnCheckOn="blabla" OnCheckOff="blabla" OnOverButton="blabla"
    // OnSaveValue="blabl" OnLoadValue="" Literal="blabla" widthOffset="" heightOffset=""/>
    // <texture name="q3dm1" on="gui_q3dm1_on.jpg" off="gui_q3dm1_off.jpg" deactivated="gui_q3dm1_on.jpg" />
    // <texture name="q3dm2" on="gui_q3dm2_on.jpg" off="gui_q3dm2_off.jpg" deactivated="gui_q3dm2_on.jpg" />
    // ...
    //</RadioBox>


    std::string name = pNewNode.GetPszProperty( "name", "defaultGuiElement" );
    float  posx = pNewNode.GetFloatProperty( "posx", 0.f );
    float  posy = pNewNode.GetFloatProperty( "posy", 0.f );
    float  w = pNewNode.GetFloatProperty( "width", 50.f );
    float  h = pNewNode.GetFloatProperty( "height", 50.f );
    bool visible = pNewNode.GetBoolProperty( "visible", true );
    bool activated  = pNewNode.GetBoolProperty( "active", true );
    std::string default_checkButton = pNewNode.GetPszProperty( "default_checkButton", "" );
    std::string texture_back = pNewNode.GetPszProperty( "texture_back", "" );
    uint32 columns = pNewNode.GetAttribute<int32>( "columns", 0 );
    uint32 rows = pNewNode.GetAttribute<int32>( "rows", 0 );
    std::string OnCheckOn  = pNewNode.GetPszProperty( "OnCheckOn", "" );
    std::string OnCheckOff  = pNewNode.GetPszProperty( "OnCheckOff", "" );
    std::string OnOverButton = pNewNode.GetPszProperty( "OnOverButton", "" );
    std::string OnSaveValue = pNewNode.GetPszProperty( "OnSaveValue", "" );
    std::string OnLoadValue = pNewNode.GetPszProperty( "OnLoadValue", "" );
    std::string l_literal  = pNewNode.GetPszProperty( "Literal", "" );
    float  widthOffsetPercent = pNewNode.GetFloatProperty( "widthOffset", 0.f );
    float  heightOffsetPercent = pNewNode.GetFloatProperty( "heightOffset", 0.f );

    uint32 widthOffset = ( uint32 )( screenResolution.x * 0.01f * widthOffsetPercent );
    uint32 heightOffset = ( uint32 )( screenResolution.y * 0.01f * heightOffsetPercent );

    CRadioBox* radioBox = new CRadioBox( screenResolution.y, screenResolution.x, h, w, Math::Vect2f( posx, posy ), columns,
                                         rows, default_checkButton,
                                         l_literal, heightOffset, widthOffset, visible, activated );

    if ( texture_back != "" )
    {
        CTexture* quad = tm->GetTexture( texture_back ); //GetTexture(texture_back);
        radioBox->SetTextureBack( quad );
    }

    radioBox->SetName( name );
    radioBox->SetOnLoadValueAction( OnLoadValue );
    radioBox->SetOnSaveValueAction( OnSaveValue );
    radioBox->SetCheckButtonActions( OnCheckOn, OnCheckOff, OnOverButton );
    int count = pNewNode.GetNumChildren();

    for ( int j = 0; j < count; ++j )
    {
        CXMLTreeNode pTexture = pNewNode( j );
        std::string tagName = pTexture.GetName();

        if ( tagName.compare( "texture" ) == 0 )
        {
            std::string name = pTexture.GetPszProperty( "name" );
            std::string on  = pTexture.GetPszProperty( "on" );
            std::string off = pTexture.GetPszProperty( "off" );
            std::string deactivated = pTexture.GetPszProperty( "deactivated" );

            CTexture* texture_on  = tm->GetTexture( on ); //GetTexture(on);
            CTexture* texture_off = tm->GetTexture( off );
            CTexture* texture_deactivated = tm->GetTexture( deactivated );
            radioBox->SetCheckButton( name, texture_on, texture_off, texture_deactivated );
        }
    }

    return radioBox;
}

CImage* CWindows::_LoadImage( CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution, CTextureManager* TM )
{
    //<Image name="imageRoomSelected_gameserver2" posx="35" posy="20" height="40" width="30" visible="true" active="true"
    //default="q3dm1" isAnimated="true", time="0" loop="true" OnSaveValue="blabla" OnLoadValue="blabla" Literal="blabla"
    //widthOffset="" heightOffset="" flip="" Literal="blabla" widthOffset="" heightOffset=""/>
    // <texture name="q3dm1" texture="gui_q3dm1_off.jpg"/>
    // <texture name="q3dm2" texture="gui_q3dm2_off.jpg"/>
    // ...
    //</Image>
    float posx  = pNewNode.GetFloatProperty( "posx", 0.f );
    float posy  = pNewNode.GetFloatProperty( "posy", 0.f );
    float w  = pNewNode.GetFloatProperty( "width", 50.f );
    float h  = pNewNode.GetFloatProperty( "height", 50.f );
    bool visible = pNewNode.GetBoolProperty( "visible", true );
    bool activated = pNewNode.GetBoolProperty( "active", true );
    const std::string& name  = pNewNode.GetPszProperty( "name", "defaultGuiElement" );
    const std::string& default_image = pNewNode.GetPszProperty( "default", "" );
    const std::string& OnSaveValue  = pNewNode.GetPszProperty( "OnSaveValue", "" );
    const std::string& OnLoadValue  = pNewNode.GetPszProperty( "OnLoadValue", "" );
    const std::string& flip = pNewNode.GetPszProperty( "flip", "" );
    bool backGround  = pNewNode.GetBoolProperty( "backGround", false );
    const std::string& l_literal  = pNewNode.GetPszProperty( "Literal", "" );
    float widthOffsetPercent = pNewNode.GetFloatProperty( "widthOffset", 0.f );
    float heightOffsetPercent = pNewNode.GetFloatProperty( "heightOffset", 0.f );

    uint32 widthOffset = ( uint32 )( screenResolution.x * 0.01f * widthOffsetPercent );
    uint32 heightOffset = ( uint32 )( screenResolution.y * 0.01f * heightOffsetPercent );

    CImage* image = new CImage( screenResolution.y, screenResolution.x, h, w, Math::Vect2f( posx, posy ),
                                l_literal, heightOffset, widthOffset, visible, activated );
    image->SetName( name );
    image->SetActiveTexture( default_image );

    image->SetOnLoadValueAction( OnLoadValue );
    image->SetOnSaveValueAction( OnSaveValue );
    image->SetBackGround( backGround );

    if ( flip.compare( "FLIP_X" ) == 0 )
    {
        image->SetFlip( CGraphicsManager::FLIP_X );
    }
    else if ( flip.compare( "FLIP_Y" ) == 0 )
    {
        image->SetFlip( CGraphicsManager::FLIP_Y );
    }
    else
    {
        image->SetFlip( CGraphicsManager::NONE_FLIP );
    }

    for ( int j = 0, count = pNewNode.GetNumChildren(); j < count; ++j )
    {
        CXMLTreeNode pTexture = pNewNode( j );
        const std::string& tagName = pTexture.GetName();

        if ( tagName.compare( "texture" ) == 0 )
        {
            const std::string& name  = pNewNode( j ).GetPszProperty( "name" );
            const std::string& texture = pNewNode( j ).GetPszProperty( "name_texture" );
            CTexture* texture_image  = TM->GetTexture( texture );
            image->SetTexture( texture_image, name );
        }
    }

    return image;
}



CProgressBar* CWindows::LoadProgressBar( CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution,
        CTextureManager* TM )
{
    //<ProgressBar name="progressBar1" posx="200" posy="600" height="10" width="100" visible="true" active="true"
    // texture_back="hola" texture_bar="hola2" id_font="0" color_font_r="0" color_font_g="0" color_font_b="0"
    // Literal="blabla" widthOffset="" heightOffset="" OnComplete="blabla"/>

    std::string name = pNewNode.GetPszProperty( "name", "defaultGuiElement" );
    float  posx = pNewNode.GetFloatProperty( "posx", 0.f );
    float  posy = pNewNode.GetFloatProperty( "posy", 0.f );
    float  w = pNewNode.GetFloatProperty( "width", 50.f );
    float  h = pNewNode.GetFloatProperty( "height", 50.f );
    bool visible = pNewNode.GetBoolProperty( "visible", true );
    bool activated  = pNewNode.GetBoolProperty( "active", true );
    std::string texture_bar = pNewNode.GetPszProperty( "texture_bar", "" );
    std::string texture_back = pNewNode.GetPszProperty( "texture_back", "" );
    std::string OnComplete  = pNewNode.GetPszProperty( "OnComplete", "" );
    uint32 idFont  = pNewNode.GetAttribute<int32>( "id_font", 0 );
    float  color_font_r = pNewNode.GetFloatProperty( "color_font_r", 0.f );
    float  color_font_g = pNewNode.GetFloatProperty( "color_font_g", 0.f );
    float  color_font_b = pNewNode.GetFloatProperty( "color_font_b", 0.f );
    std::string l_literal  = pNewNode.GetPszProperty( "Literal", "" );
    float  widthOffsetPercent = pNewNode.GetFloatProperty( "widthOffset", 0.f );
    float  heightOffsetPercent = pNewNode.GetFloatProperty( "heightOffset", 0.f );

    CTexture* bar = TM->GetTexture( texture_bar );
    CTexture* back = TM->GetTexture( texture_back );

    uint32 widthOffset = ( uint32 )( screenResolution.x * 0.01f * widthOffsetPercent );
    uint32 heightOffset = ( uint32 )( screenResolution.y * 0.01f * heightOffsetPercent );

    CProgressBar* progressBar = new CProgressBar( screenResolution.y, screenResolution.x, h, w, Math::Vect2f( posx, posy ),
            l_literal, heightOffset, widthOffset, visible, activated );
    progressBar->SetName( name );
    progressBar->SetTextures( back, bar );
    progressBar->SetFont( idFont, Math::CColor( color_font_r, color_font_g, color_font_b ) );
    progressBar->SetOnComplete( OnComplete );
    return progressBar;
}

CStaticText* CWindows::LoadStaticText( CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution,
                                       CTextureManager* tm )
{
    //<StaticText name="pepito" posx="0" posy="2" literal="blabla" visible="true" active="true"/>

    const std::string& name = pNewNode.GetPszProperty( "name", "defaultGuiElement" );
    float posx = pNewNode.GetFloatProperty( "posx", 0.f );
    float posy = pNewNode.GetFloatProperty( "posy", 0.f );
    float w = pNewNode.GetFloatProperty( "width", 50.f );
    float h = pNewNode.GetFloatProperty( "height", 50.f );
    bool visible  = pNewNode.GetBoolProperty( "visible", true );
    bool activated  = pNewNode.GetBoolProperty( "active", true );
    const std::string& l_literal = pNewNode.GetPszProperty( "literal", "" );

    CStaticText* staticText = new CStaticText( screenResolution.y, screenResolution.x, h, w, Math::Vect2f( posx, posy ),
            l_literal, visible, activated );
    staticText->SetName( name );
    return staticText;
}

CMap* CWindows::LoadMap( CXMLTreeNode& pNewNode, const Math::Vect2i& screenResolution )
{
    //<Map name="NameMap" pos_in_screen="0 90" width="8" height="40" texture_marco="Data/textures/GUI/Textures_Test/marco2.png" texture_map="Data/textures/mapa.png"
    //width_map="0.02" height_map="0.02" pos_0_0_3d_map="350.0 550.0" pos_1_1_3d_map="-50.0 -80.0" visible="true" active="true">
    //mark name="player" texture="Data/textures/GUI/Textures_Test/flecha.png" width="20" height="20" get_position_script="get_player_position()" orientation="get_player_orientation_on_map()"/>

    const std::string& name                     = pNewNode.GetPszProperty( "name", "Radar" );
    const std::string& texture_marco            = pNewNode.GetPszProperty( "texture_marco",
            "Data/textures/GUI/Textures_Test/marco2.png" );
    const std::string& texture_map              = pNewNode.GetPszProperty( "texture_map",
            "Data/textures/GUI/Textures_Test/mapa.png" );
    const std::string& position_script_enemy    = pNewNode.GetPszProperty( "get_position_script", "no_script" );
    const std::string& orientation_script_enemy = pNewNode.GetPszProperty( "orientation", "no_script" );
    const std::string& texture_enemy            = pNewNode.GetPszProperty( "texture_enemy", "" );
    Math::Vect2f pos_in_screen                  = pNewNode.GetVect2fProperty( "pos_in_screen", Math::Vect2f( 0.f, 0.f ) );
    Math::Vect2f pos_0_0_3d_map                 = pNewNode.GetVect2fProperty( "pos_0_0_3d_map", Math::Vect2f( 0.f, 0.f ) );
    Math::Vect2f pos_1_1_3d_map                 = pNewNode.GetVect2fProperty( "pos_1_1_3d_map", Math::Vect2f( 0.f, 0.f ) );
    float w                                     = pNewNode.GetFloatProperty( "width", 8.f );
    float h                                     = pNewNode.GetFloatProperty( "height", 40.f );
    float w_map                                 = pNewNode.GetFloatProperty( "width_map", 0.02f );
    float h_map                                 = pNewNode.GetFloatProperty( "height_map", 0.02f );
    bool visible                                = pNewNode.GetBoolProperty( "visible", true );
    bool activated                              = pNewNode.GetBoolProperty( "active", true );
    uint32 WidthEnemy                           = pNewNode.GetAttribute<int32>( "width_enemy", 50 );
    uint32 HeightEnemy                          = pNewNode.GetAttribute<int32>( "height_enemy", 50 );


    CMap* l_Map = new CMap( screenResolution.y, screenResolution.x, h, w, pos_in_screen, texture_marco, texture_map,
                            pos_0_0_3d_map, pos_1_1_3d_map,
                            h_map, w_map, visible, activated );
    l_Map->SetName( name );

    l_Map->AddEnemys( texture_enemy, WidthEnemy, HeightEnemy, position_script_enemy, orientation_script_enemy );

    int count = pNewNode.GetNumChildren();

    for ( int i = 0; i < count; ++i )
    {
        CXMLTreeNode pSubNewNode              = pNewNode( i );
        const std::string& NameItem           = pSubNewNode.GetPszProperty( "name", "defaultItemElement" );
        const std::string& TextureItem        = pSubNewNode.GetPszProperty( "texture", "no_texture" );
        const std::string& position_script    = pSubNewNode.GetPszProperty( "get_position_script", "no_script" );
        const std::string& orientation_script = pSubNewNode.GetPszProperty( "orientation", "no_script" );
        uint32 WidthItem                      = pSubNewNode.GetAttribute<int32>( "width", 50 );
        uint32 HeightItem                     = pSubNewNode.GetAttribute<int32>( "height", 50 );
        float Yaw                             = pSubNewNode.GetFloatProperty( "yaw", 0.f );

        std::string tagName = pSubNewNode.GetName();

        if ( tagName.compare( "item" ) == 0 )
        {
            Math::Vect3f pos = pSubNewNode.GetVect3fProperty( "pos_in_map", Math::Vect3f( 0.f, 0.0f, 0.f ) );
            l_Map->AddItem( NameItem, TextureItem, pos, WidthItem, HeightItem, Yaw, position_script, orientation_script );
        }

        if ( tagName.compare( "mark_player" ) == 0 )
        {
            Math::Vect3f pos = pSubNewNode.GetVect3fProperty( "pos_in_map", Math::Vect3f( 0.f, 0.0f, 0.f ) );
            l_Map->AddPlayer( NameItem, TextureItem, pos, WidthItem, HeightItem, Yaw, position_script, orientation_script );
        }

    }

    return l_Map;
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