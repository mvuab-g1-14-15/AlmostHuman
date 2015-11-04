#include "Cinematics\CinematicsElement\HideElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "GUIManager.h"
#include "Widgets\TextBox.h"
#include "Cinematics\CinematicManager.h"
#include "Timer\Timer.h"

CHideElement::CHideElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
    , m_Type( atts.GetAttribute<std::string>( "type",  "no_type" ) )
{
}

void CHideElement::Update()
{
    if( m_Type == "show_dialog" )
    {
        //  GUIInstance->GetTextBox()->SetVisible(false);
        //  GUIInstance->GetTextBox()->SetActive(false);
    }
    m_CurrentTime += constFrameTime;
}

void CHideElement::Execute( CGraphicsManager& GM )
{
}

void CHideElement::Render()
{
}

void CHideElement::Render2D(CGraphicsManager &GM)
{
}