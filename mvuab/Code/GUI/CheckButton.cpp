#include "__PCH_GUI.h"

#include "CheckButton.h"
#include "InputManager.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "Logger/Logger.h"
#include "ScriptManager.h"
#include "EngineManagers.h"


//---Constructor
CCheckButton::CCheckButton(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
														const Math::Vect2f position_percent, bool isOn, std::string lit, uint32 textHeightOffset,
														uint32 textWidthOffset, bool isVisible, bool isActive)
: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, CHECKBUTTON, lit, textHeightOffset, textWidthOffset, isVisible,isActive)
, m_pOnTexture(NULL)
, m_pOffTexture(NULL)
, m_pDeactivatedTexture(NULL)
, m_sLuaCode_OnCheckOn("")
, m_sLuaCode_OnCheckOff("")
, m_sLuaCode_OnOver("")
, m_OnColor(Math::colGREEN)
, m_OffColor(Math::colRED)
, m_DeactivatedColor(colYELLOW)
{
	if (isOn)
		m_eState = CBS_ON;
	else
		m_eState = CBS_OFF;
}

//---------------Interfaz de CGuiElement----------------------
void CCheckButton::Render ()
{
	if( CGuiElement::m_bIsVisible)
	{
		//Primero renderizamos todos los hijos que pudiera tener el checkBbutton:
		CGuiElement::Render();

		if( CGuiElement::m_bIsActive )
		{
			if( m_eState == CBS_ON )
			{
				if (m_pOnTexture)
				{
					GraphicsInstance->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_pOnTexture);
				}
				else
				{
					GraphicsInstance->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_OnColor);
				}
			}
			else
			{
				if (m_pOnTexture)
				{
					GraphicsInstance->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_pOffTexture);
				}
				else
				{
					GraphicsInstance->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_OffColor);
				}
			}
		}
		else
		{
			if (m_pDeactivatedTexture)
			{
				//GraphicsInstance->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_pDeactivatedTexture);
			}
			else
			{
				GraphicsInstance->DrawQuad2D(CGuiElement::m_Position,CGuiElement::m_uWidth,CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_DeactivatedColor);
			}
		}

		//Finalmente renderizamos el texto:
		CGuiElement::RenderText();	

	} //END if( CGuiElement::m_bIsVisible )
}

void CCheckButton::Update ()
{

	if( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
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

			if (InputManagerInstance->IsUpDown(IDV_MOUSE,MOUSE_BUTTON_LEFT))
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
	m_pOnTexture					= on;
	m_pOffTexture					= off;
	m_pDeactivatedTexture	= deactivated;
}

void CCheckButton::SetColors (const Math::CColor& on, const Math::CColor& off, const Math::CColor& deactivated, float alpha)
{
	m_OnColor						= on;
	m_OffColor					= off;
	m_DeactivatedColor	= deactivated;

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
		m_pParent->OnClickedChild(CGuiElement::m_sName);

	if (m_sLuaCode_OnCheckOn.compare(""))
	{
		//Lanzar acción en Lua:
		//TODO RAUL
		//CScriptManager* scriptManager = CORE->GetScriptManager();
		ScriptMInstance->RunCode(m_sLuaCode_OnCheckOn);
	}
}

void CCheckButton::OnCheckOff ()
{
	if( CGuiElement::m_pParent != NULL )
		m_pParent->OnClickedChild(CGuiElement::m_sName);

	if (m_sLuaCode_OnCheckOff.compare(""))
	{
		//Lanzar acción en Lua:
		//TODO RAUL
		//CScriptManager* scriptManager = CORE->GetScriptManager();
		ScriptMInstance->RunCode(m_sLuaCode_OnCheckOff);
	}
}

void CCheckButton::OnOverButton ()
{
	if (m_sLuaCode_OnOver.compare(""))
	{
		//Lanzar acción en Lua:
		//TODO RAUL
		//CScriptManager* scriptManager = CORE->GetScriptManager();
		ScriptMInstance->RunCode(m_sLuaCode_OnOver);
	}
}
