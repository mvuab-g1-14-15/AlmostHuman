

#include "MousePointer\PointerMouse.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "EngineManagers.h"
#include "Timer\Timer.h"
#include "Core.h"

//---Constructor
CPointerMouse::CPointerMouse(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
															const Math::Vect2f position_percent, bool isVisible, bool isActive)
: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, IMAGE, "",0,0, isVisible,isActive)
, m_sActiveTexture( "default" )
, m_bAnimated(false)
, m_bLoop(false)
, m_fTime(0.f)
, m_fCounter(0.f)
, m_Alignment(CGraphicsManager::CENTER)
, m_bIsQuadrant(false)
{}


//---------------CGuiElement Interface----------------------
void CPointerMouse::Render ()
{
	if( CGuiElement::m_bIsVisible)
	{
		tTexturesMap::iterator it = m_Textures.find(m_sActiveTexture);
		if( it != m_Textures.end() )
		{
			CTexture* texture = it->second;
			assert(texture);
			uint32 w = CGuiElement::m_uWidth;
			uint32 h = CGuiElement::m_uHeight;
			if (m_bIsQuadrant)
			{
				h = w;
			}
			//TODO RAUL
			GraphicsInstance->DrawQuad2D(CGuiElement::m_Position, w, h, m_Alignment, texture);
		}
	}
}

void CPointerMouse::Update ()
{
	if( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
	{
		Math::Vect2i pos;
		//inputManager->GetPosition(IDV_MOUSE, pos);
		InputManagerInstance->GetPosition(IDV_GAMEPAD1, pos);
		CGuiElement::SetPosition(pos);

		if (m_bAnimated)
		{
			m_fCounter += deltaTime;
			if (m_fCounter > m_fTime)
			{
				tTexturesMap::iterator it = m_Textures.find(m_sActiveTexture);
				it++;
				if (it == m_Textures.end())
				{
					it = m_Textures.begin();
				}
				m_sActiveTexture = it->first;
				m_fCounter = 0.f;
			}
		}//END if (m_bAnimated)

	}//END if( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
}

//---------------CImage Interface----------------------


void CPointerMouse::SetTexture (CTexture* texture, std::string name )
{
	m_Textures.insert( std::pair<std::string,CTexture*>(name, texture) );
}


