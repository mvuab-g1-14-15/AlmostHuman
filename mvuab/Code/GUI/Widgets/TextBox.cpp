#include "Core.h"
#include "Widgets\TextBox.h"
#include "GraphicsManager.h"
#include "Fonts\FontManager.h"
#include "EngineManagers.h"


//---Constructor
CTextBox::CTextBox(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float width_percent,
										const Math::Vect2f position_percent,  float buttonWidthPercent, float buttonHeightPercent,
										std::string lit, uint32 textHeightOffset, uint32 textWidthOffset,bool isVisible, bool isActive)
: CDialogBox(	windowsHeight, windowsWidth, height_precent, width_percent, position_percent, buttonWidthPercent, 
							buttonHeightPercent, lit, textHeightOffset, textWidthOffset, isVisible, isActive)
, m_sMessage("Default_TextBox")
, m_uFontID(0)
, m_TextColor(Math::colBLACK)

{}


void CTextBox::Render ()
{

	if (CGuiElement::m_bIsVisible)
	{
		CDialogBox::Render();
		//Pintamos el texto
		FontInstance->DrawText(	CGuiElement::m_Position.x+20, CGuiElement::m_Position.y + (uint32)(CGuiElement::m_uHeight*0.4f), 
									m_TextColor, m_uFontID, m_sMessage.c_str());

	}
}

void CTextBox::SetFont (Math::CColor textColor, uint32 fontID)
{
	m_uFontID = fontID;
	m_TextColor = textColor;
}
