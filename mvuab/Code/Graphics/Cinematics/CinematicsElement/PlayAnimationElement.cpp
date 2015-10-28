#include "Cinematics\CinematicsElement\PlayAnimationElement.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Cinematics\Cinematic.h"
#include "Timer\Timer.h"
#include "RenderableObject\Scene.h"
#include "RenderableObject\Room.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "AnimatedModels\AnimatedInstanceModel.h"

CPlayAnimationElement::CPlayAnimationElement( CXMLTreeNode& atts ) : CCinematicsElement( atts )
	, m_Animated( atts.GetAttribute<bool>("animated", false) )
    , m_Playing( false )
{
	if(m_Animated)
	{
		m_pModeloAnimado = dynamic_cast<CAnimatedInstanceModel*>(SceneInstance->GetResource( atts.GetAttribute<std::string>("room", "") )->GetLayer("solid")->GetResource( m_Name ));
		m_Animation = atts.GetAttribute<std::string>("animation", "");
	}
	else
	{
		m_Cinematic = new CCinematic(atts.GetAttribute<std::string>( "resource_id_file",  "no_file" ) );
	}
}

CPlayAnimationElement::~CPlayAnimationElement()
{
	if(!m_Animated)
    {
        CHECKED_DELETE(m_Cinematic);
    }
}


void CPlayAnimationElement::Update()
{
	if( m_Animated)
	{
		m_pModeloAnimado->SetActive(true);
		m_pModeloAnimado->SetYaw(m_pModeloAnimado->GetYaw() + Math::pi32);
		m_pModeloAnimado->MakeTransform();
		m_pModeloAnimado->ChangeAnimation(m_Animation, 0.2f, 0.0f);
		m_CurrentTime += deltaTimeMacro;
	}
	else
	{
		if( !m_Playing )
		{
			m_Cinematic->Play(false);
			m_Playing = true;
		}
		m_Cinematic->Update();

		m_CurrentTime += deltaTimeMacro;
		if( m_CurrentTime >= m_Time )
		{
			m_IsBlocker = false;
			m_Playing = false;
		}
	}
}

void CPlayAnimationElement::Execute( CGraphicsManager& GM )
{

}

void CPlayAnimationElement::Render()
{
    m_Cinematic->Render();
}

void CPlayAnimationElement::Render2D(CGraphicsManager &GM)
{
}