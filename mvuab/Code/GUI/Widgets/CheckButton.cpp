#include "Widgets\CheckButton.h"
#include "InputManager.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "Logger/Logger.h"
#include "ScriptManager.h"
#include "EngineManagers.h"

CCheckButton::CCheckButton( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution )
    : CGuiElement( aNode, screenResolution )
    , m_pOnTexture(aNode.GetAttribute<CTexture>( "texture_on" ))
    , m_pOffTexture(aNode.GetAttribute<CTexture>( "texture_off" ))
    , m_pDeactivatedTexture(aNode.GetAttribute<CTexture>( "texture_deactivated" ))
    , m_sLuaCode_OnCheckOn(aNode.GetAttribute<std::string>( "OnCheckOn", "" ))
    , m_sLuaCode_OnCheckOff(aNode.GetAttribute<std::string>( "OnCheckOff", "" ))
    , m_sLuaCode_OnOver(aNode.GetAttribute<std::string>( "OnOverButton", "" ))
    , m_OnColor(Math::colGREEN)
    , m_OffColor(Math::colRED)
    , m_DeactivatedColor(Math::colYELLOW)
    , m_eState( aNode.GetAttribute<bool>( "isOn", "" ) ? CBS_ON : CBS_OFF )
{
    m_eType = CHECKBUTTON;
}

//---------------Interfaz de CGuiElement----------------------
void CCheckButton::Render ()
{
    if( GetVisible())
    {
        //Primero renderizamos todos los hijos que pudiera tener el checkBbutton:
        CGuiElement::Render();

        if( CGuiElement::m_bIsActive )
        {
            if( m_eState == CBS_ON )
            {
                if (m_pOnTexture)
                {
                    //TODO RAUL
                    GraphicsInstance->DrawQuad2D(CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight,
                                                 CGraphicsManager::UPPER_LEFT, m_pOnTexture);
                }
                else
                {
                    GraphicsInstance->DrawQuad2D(CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight,
                                                 CGraphicsManager::UPPER_LEFT, m_OnColor);
                }
            }
            else
            {
                if (m_pOnTexture)
                {
                    //TODO RAUL
                    GraphicsInstance->DrawQuad2D(CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight,
                                                 CGraphicsManager::UPPER_LEFT, m_pOffTexture);
                }
                else
                {
                    GraphicsInstance->DrawQuad2D(CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight,
                                                 CGraphicsManager::UPPER_LEFT, m_OffColor);
                }
            }
        }
        else
        {
            if (m_pDeactivatedTexture)
            {
                GraphicsInstance->DrawQuad2D(CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight,
                                             CGraphicsManager::UPPER_LEFT, m_pDeactivatedTexture);
            }
            else
            {
                GraphicsInstance->DrawQuad2D(CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight,
                                             CGraphicsManager::UPPER_LEFT, m_DeactivatedColor);
            }
        }

        //Finalmente renderizamos el texto:
        CGuiElement::RenderText();

    } //END if( GetVisible() )
}

void CCheckButton::Update ()
{

    if( GetVisible() && CGuiElement::m_bIsActive )
    {
        //Primero actualizamos todos los hijos que pudiera tener el checkButton:
        CGuiElement::Update();

        Math::Vect2i mousePosition;
        InputManagerInstance->GetPosition(IDV_MOUSE, mousePosition);
        CGuiElement::CalculatePosMouse(mousePosition);

        if( CGuiElement::IsInside() )
        {
            if( CGuiElement::IsOver() )
            {
                OnOverButton();
            }

            if (InputManagerInstance->IsUpDown(IDV_MOUSE, MOUSE_BUTTON_LEFT))
            {
                if (m_eState == CBS_ON)
                {
                    m_eState = CBS_OFF;
                    OnCheckOff();
                }
                else
                {
                    m_eState = CBS_ON;
                    OnCheckOn();
                }
            }
        }
    }
}

//---------------Interfaz de CheckButton---------------------------
void CCheckButton::SetTextures (CTexture* on, CTexture* off, CTexture* deactivated)
{
    m_pOnTexture                    = on;
    m_pOffTexture                    = off;
    m_pDeactivatedTexture    = deactivated;
}

void CCheckButton::SetColors (const Math::CColor& on, const Math::CColor& off, const Math::CColor& deactivated,
                              float alpha)
{
    m_OnColor                        = on;
    m_OffColor                    = off;
    m_DeactivatedColor    = deactivated;

    m_OnColor.SetAlpha(alpha);
    m_OnColor.SetAlpha(alpha);
    m_DeactivatedColor.SetAlpha(alpha);
}

void CCheckButton::SetOnCheckOnAction (const std::string& inAction)
{
    m_sLuaCode_OnCheckOn = inAction;
}

void CCheckButton::SetOnCheckOffAction (const std::string& inAction)
{
    m_sLuaCode_OnCheckOff = inAction;
}

void CCheckButton::SetOnOverAction (const std::string& inAction)
{
    m_sLuaCode_OnOver = inAction;
}

void CCheckButton::OnCheckOn ()
{
    if( CGuiElement::m_pParent != NULL )
    {
        m_pParent->OnClickedChild(GetName());
    }

    if (m_sLuaCode_OnCheckOn.compare(""))
    {
        //Lanzar acción en Lua:
        //TODO RAUL
        ////TODO RAUL SCRIPT//CScriptManager* scriptManager = CORE->GetScriptManager();
        ScriptMInstance->RunCode(m_sLuaCode_OnCheckOn);
    }
}

void CCheckButton::OnCheckOff ()
{
    if( CGuiElement::m_pParent != NULL )
    {
        m_pParent->OnClickedChild(GetName());
    }

    if (m_sLuaCode_OnCheckOff.compare(""))
    {
        //Lanzar acción en Lua:
        //TODO RAUL
        ////TODO RAUL SCRIPT//CScriptManager* scriptManager = CORE->GetScriptManager();
        ScriptMInstance->RunCode(m_sLuaCode_OnCheckOff);
    }
}

void CCheckButton::OnOverButton ()
{
    if (m_sLuaCode_OnOver.compare(""))
    {
        //Lanzar acción en Lua:
        //TODO RAUL
        ////TODO RAUL SCRIPT//CScriptManager* scriptManager = CORE->GetScriptManager();
        ScriptMInstance->RunCode(m_sLuaCode_OnOver);
    }
}
