#include "StaticText.h"
#include "GraphicsManager.h"
#include "Logger/Logger.h"
#include "EngineManagers.h"

//---Constructor
CStaticText::CStaticText( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
													const Math::Vect2f position_percent, std::string lit, bool isVisible, bool isActive)
: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, STATIC_TEXT, lit, 0, 0, isVisible,isActive)
{}

//---------------Interfaz de GuiElement----------------------
void	CStaticText::Render ()
{
	if( CGuiElement::m_bIsVisible)
	{
		//Primero renderizamos todos los hijos que pudiera tener el Static Text:
		CGuiElement::Render();

		//Finalmente renderizamos el texto:
		CGuiElement::RenderText();
	}
}

void CStaticText::Update()
{
	if( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
	{
		//Primero actualizamos todos los hijos que pudiera tener el checkButton:
		CGuiElement::Update();
	}
}

void CStaticText::SetLiteral( const std::string& lit)
{
	CGuiElement::m_sLiteral = lit;
}