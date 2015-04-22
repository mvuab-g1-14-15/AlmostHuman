#include "Widgets\DialogBox.h"
#include "InputManager.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"

//---Constructor
CDialogBox::CDialogBox(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
											 const Math::Vect2f position_percent,  float buttonWidthPercent, float buttonHeightPercent,
											 std::string lit, uint32 textHeightOffset, uint32 textWidthOffset, bool isVisible, bool isActive)
: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, DIALOG_BOX, lit, textHeightOffset, textWidthOffset, isVisible, isActive)

, m_bDialogClicked(false)
, m_ButtonClose(windowsHeight, windowsWidth, buttonHeightPercent, buttonWidthPercent,
								position_percent + Math::Vect2f(witdh_percent-buttonWidthPercent,0.f), "", 0, 0, isVisible, isActive )
, m_ButtonMove(windowsHeight, windowsWidth, buttonHeightPercent, witdh_percent-buttonWidthPercent,  position_percent, "", 0, 0, isVisible, isActive )
, m_pBackGroundTexture(NULL)
, m_BackGroundColor(Math::colWHITE)
, m_bStart_to_Move(false)
, m_PreviousPosMouse(Math::v2iZERO)
, m_fButtonHeight( buttonHeightPercent )
, m_fButtonWidth( buttonWidthPercent )
{
	m_ButtonClose.SetParent(this);
	m_ButtonClose.SetName("buttonClose");
}

//---------------Interfaz de GuiElement----------------------
void CDialogBox::Render	()
{
	if (CGuiElement::m_bIsVisible)
	{
		if (m_pBackGroundTexture)
		{
			//TODO RAUL CREAR METODO DRAWQUAD2D CON TEXTURA
				GraphicsInstance->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_pBackGroundTexture);
		}
		else
		{
				GraphicsInstance->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_BackGroundColor);
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
			if (!InputManagerInstance->IsDown(IDV_MOUSE,MOUSE_BUTTON_LEFT))
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
			if (InputManagerInstance->IsDown(IDV_KEYBOARD,0))
			{
				CGuiElement::m_bFocus = true;
			}
		}

		m_ButtonClose.Update();
		m_ButtonMove.Update();

		CGuiElement::CalculatePosMouse(posMouse);
		if( m_ButtonMove.IsClicking() )
			m_bDialogClicked = true;
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
	m_ButtonClose.SetPosition( pos + Math::Vect2i((uint32)(CGuiElement::m_uWidth - m_ButtonClose.GetWidth()),0) );
	CGuiElement::SetPosition( pos );
}

//---------------Interfaz de Button---------------------------
void CDialogBox::SetCloseButtonTextures	(CTexture* normal, CTexture* over, CTexture* clicked, CTexture* deactivated)
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

void CDialogBox::SetCloseButtonColors (	const Math::CColor& normal, const Math::CColor& over, const Math::CColor& clicked,
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