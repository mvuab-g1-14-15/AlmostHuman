
#include "Widgets\TextBox.h"
#include "GraphicsManager.h"
#include "Fonts\FontManager.h"
#include "EngineManagers.h"

CTextBox::CTextBox( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution )
    : CDialogBox( aNode, screenResolution )
    , m_sMessage("")
    , m_uFontID(0)
    , m_TextColor(Math::colBLACK)
{
}

void CTextBox::Render ()
{

    if (CGuiElement::m_bIsVisible)
    {
        CDialogBox::Render();
        //Pintamos el texto
        FontInstance->DrawText( CGuiElement::m_Position.x + 20,
                                CGuiElement::m_Position.y + (uint32)(CGuiElement::m_uHeight * 0.4f),
                                m_TextColor, m_uFontID, m_sMessage.c_str());

    }
}

void CTextBox::SetFont (Math::CColor textColor, uint32 fontID)
{
    m_uFontID = fontID;
    m_TextColor = textColor;
}
