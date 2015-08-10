

#include "StaticText.h"
#include "GraphicsManager.h"
#include "Logger/Logger.h"
#include "EngineManagers.h"

CStaticText::CStaticText( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution )
    : CGuiElement( aNode, screenResolution )
{

}

//---------------Interfaz de GuiElement----------------------
void    CStaticText::Render ()
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