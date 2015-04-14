#include "__PCH_GUI.h"

#include "Image.h"
#include "Texture/Texture.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Core.h"
#include "Timer\Timer.h"

//---Constructor
CImage::CImage(	uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
							 const Math::Vect2f position_percent, std::string lit, uint32 textHeightOffset, uint32 textWidthOffset, 
							 bool isVisible, bool isActive)
: CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, IMAGE, lit, textHeightOffset, textWidthOffset, isVisible,isActive)
, m_sActiveTexture( "default" )
, m_bAnimated(false)
, m_bLoop(false)
, m_fTimePerImage(0.f)
, m_fCounter(0.f)
, m_Color(Math::colBLACK)
, m_eFlip(CGraphicsManager::NONE_FLIP)
, m_bIsQuadrant(false)
, m_bIsBackGround(false)
{}


//---------------CGuiElement Interface----------------------
void CImage::Render ()
{
	if( CGuiElement::m_bIsVisible )
	{
		CTexture* texture = NULL;
		if (m_bAnimated)
		{
			texture = *m_itVecTextures;
		}
		else
		{
			tTexturesMap::iterator it = m_Textures.find(m_sActiveTexture);
			if (it != m_Textures.end())
			{
				texture = it->second;
			}
		}
		
		if (texture)
		{
			uint32 w = CGuiElement::m_uWidth;
			uint32 h = CGuiElement::m_uHeight;
			if (m_bIsQuadrant)
				h = CGuiElement::m_uWidth;
			//TODO RAUL
			//if (texture)
				//GraphicsInstance->DrawQuad2D(CGuiElement::m_Position,w, h,  CGraphicsManager::UPPER_LEFT, texture, m_eFlip);
			//else
				GraphicsInstance->DrawQuad2D(CGuiElement::m_Position, w, h,  CGraphicsManager::UPPER_LEFT, m_Color);
		}

		//Finalmente renderizamos el texto:
		CGuiElement::RenderText();	
	}
	
}

void CImage::Update ()
{
	if (m_bAnimated)
	{
		m_fCounter += deltaTime;
		if (m_fCounter > m_fTimePerImage)
		{
			m_itVecTextures++;
			if (m_itVecTextures == m_VecTextures.end())
			{
				m_itVecTextures = m_VecTextures.begin();
				if (!m_bLoop)
				{
					m_bAnimated = false;	
				}
			}
			m_fCounter = 0.f;
		}
	}
}

//---------------CImage Interface----------------------
void CImage::PlayAnimation (float timePerImage, bool loop)
{
	assert(m_Textures.size()>0);

	m_bAnimated			= true;
	m_bLoop					= loop;
	m_fTimePerImage	= timePerImage;
	m_itVecTextures	= m_VecTextures.begin();
	m_fCounter			= 0.f;
}

void CImage::SetTexture (CTexture* texture, std::string name )
{
	m_Textures.insert( std::pair<std::string,CTexture*>(name, texture) );
	m_VecTextures.push_back(texture);
}


