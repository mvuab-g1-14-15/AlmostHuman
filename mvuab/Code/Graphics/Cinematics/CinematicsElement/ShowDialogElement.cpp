#include "Cinematics\CinematicsElement\ShowDialogElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "GUIManager.h"
#include "Timer\Timer.h"

CShowDialogElement::CShowDialogElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
    , m_Text( atts.GetAttribute<std::string>( "text", "" ) )
{
}

void CShowDialogElement::Update()
{
    GUIInstance->SetMessageBox(m_Text);
    m_CurrentTime += deltaTimeMacro;
}
void CShowDialogElement::Execute( CGraphicsManager& GM )
{

}

void CShowDialogElement::Render()
{
}

void CShowDialogElement::Render2D(CGraphicsManager &GM)
{
}