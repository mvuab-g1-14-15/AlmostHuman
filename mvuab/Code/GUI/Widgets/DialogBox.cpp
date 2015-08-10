

#include "Widgets\DialogBox.h"
#include "InputManager.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

CDialogBox::CDialogBox( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution )
    : CGuiElement( aNode, screenResolution )
    , m_bDialogClicked(false)
    , m_ButtonClose( aNode, screenResolution )
    , m_ButtonMove( aNode, screenResolution )
    , m_pBackGroundTexture(aNode.GetAttribute<CTexture>( "quad" ))
    , m_BackGroundColor(Math::colWHITE)
    , m_bStart_to_Move(false)
    , m_PreviousPosMouse(Math::v2iZERO)
    , m_fButtonHeight( 0.0f )
    , m_fButtonWidth( 0.0f )
{
    m_ButtonClose.SetParent(this);
    m_ButtonClose.SetName("buttonClose");
    m_ButtonClose.SetTextures
    (
        aNode.GetAttribute<CTexture>( "buttonClose_normal" ),
        aNode.GetAttribute<CTexture>( "buttonClose_over" ),
        aNode.GetAttribute<CTexture>( "buttonClose_clicked" ),
        aNode.GetAttribute<CTexture>( "buttonClose_deactivated" )
    );

    m_ButtonMove.SetParent(this);
    m_ButtonMove.SetName("buttonClose");
    m_ButtonMove.SetTextures
    (
        aNode.GetAttribute<CTexture>( "buttonMove_normal" ),
        aNode.GetAttribute<CTexture>( "buttonMove_over" ),
        aNode.GetAttribute<CTexture>( "buttonMove_clicked" ),
        aNode.GetAttribute<CTexture>( "buttonMove_deactivated" )
    );
}

//---------------Interfaz de GuiElement----------------------
void CDialogBox::Render    ()
{
    if (CGuiElement::m_bIsVisible)
    {
        if (m_pBackGroundTexture)
        {
            //TODO RAUL CREAR METODO DRAWQUAD2D CON TEXTURA
            GraphicsInstance->DrawQuad2D(CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight,
                                         CGraphicsManager::UPPER_LEFT, m_pBackGroundTexture);
        }
        else
        {
            GraphicsInstance->DrawQuad2D(CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight,
                                         CGraphicsManager::UPPER_LEFT, m_BackGroundColor);
        }

        m_ButtonMove.Render();
        m_ButtonClose.Render();
    }

    //Finalmente renderizamos el texto:
    CGuiElement::RenderText();
}

void CDialogBox::Update ()
{
    if (CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive)
    {
        Math::Vect2i posMouse;
        InputManagerInstance->GetPosition(IDV_MOUSE, posMouse);
        if( m_bDialogClicked )
        {
            if (!InputManagerInstance->IsDown(IDV_MOUSE, MOUSE_BUTTON_LEFT))
            {
                m_bDialogClicked = false;
                m_bStart_to_Move = false;
            }
            else
            {
                if( !m_bStart_to_Move )
                {
                    m_bStart_to_Move = true;
                    m_PreviousPosMouse = posMouse;
                }
                else
                {
                    Math::Vect2i incPos = posMouse - m_PreviousPosMouse;
                    m_PreviousPosMouse = posMouse;
                    m_ButtonMove.SetPosition( m_ButtonMove.GetPosition() + incPos );
                    m_ButtonClose.SetPosition( m_ButtonClose.GetPosition() + incPos );
                    SetPosition( GetPosition() + incPos);
                }
            }
        }

        CGuiElement::CalculatePosMouse(posMouse);
        if (CGuiElement::IsInside())
        {
            if (InputManagerInstance->IsDown(IDV_KEYBOARD, 0))
            {
                CGuiElement::m_bFocus = true;
            }
        }

        m_ButtonClose.Update();
        m_ButtonMove.Update();

        CGuiElement::CalculatePosMouse(posMouse);
        if( m_ButtonMove.IsClicking() )
        {
            m_bDialogClicked = true;
        }

    }// End if( GuiElement::mIsVisible && GuiElement::mIsActive )
}

void CDialogBox::OnClickedChild( const std::string& name )
{
    if( name.compare("buttonClose") == 0 )
    {
        CGuiElement::m_bIsActive = false;
        CGuiElement::m_bIsVisible = false;
    }
}

void CDialogBox::SetPosition(const Math::Vect2i& pos)
{
    m_ButtonMove.SetPosition( pos );
    m_ButtonClose.SetPosition( pos + Math::Vect2i((uint32)(CGuiElement::m_uWidth - m_ButtonClose.GetWidth()), 0) );
    CGuiElement::SetPosition( pos );
}



//---------------Interfaz de Button---------------------------
void CDialogBox::SetCloseButtonTextures    (CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated)
{
    m_ButtonClose.SetTextures(normal, over, clicked, deactivated);
}

void CDialogBox::SetMoveButtonTextures (CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated)
{
    m_ButtonMove.SetTextures(normal, over, clicked, deactivated);
}

void CDialogBox::SetDialogTexture (CTexture* background)
{
    assert(background);
    m_pBackGroundTexture = background;
}

void CDialogBox::SetCloseButtonColors (    const Math::CColor& normal, const Math::CColor& over,
        const Math::CColor& clicked,
        const Math::CColor& deactivated)
{
    m_ButtonClose.SetColors(normal, over, clicked, deactivated);
}
void CDialogBox::SetMoveButtonColors (const Math::CColor& normal, const Math::CColor& over, const Math::CColor& clicked,
                                      const Math::CColor& deactivated)
{
    m_ButtonMove.SetColors(normal, over, clicked, deactivated);
}

void CDialogBox::SetDialogColors (const Math::CColor& background)
{
    m_BackGroundColor = background;
}

