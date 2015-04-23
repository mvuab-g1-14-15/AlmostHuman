#include "Widgets\Map.h"
#include "EngineManagers.h"
#include "Texture\TextureManager.h"

//---Constructor
CMap::CMap( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
                const Math::Vect2f position_percent, std::string lit, uint32 textHeightOffset, uint32 textWidthOffset,
                bool isVisible, bool isActive)
    : CImage( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, lit,
                   textHeightOffset, textWidthOffset, isVisible, isActive )
				   , m_Character(new CImage((windowsHeight*(uint32)height_precent/100)+(windowsHeight*(uint32)position_percent.y/100), (windowsWidth*(uint32)witdh_percent/100)+(windowsWidth*(uint32)position_percent.x/100), 5, 5, Math::Vect2f(50,50), "Character",
                   textHeightOffset, textWidthOffset, isVisible, isActive))
{
	float porcentajeWidth = m_Character->GetPositionPercent().x+(position_percent.x/100*windowsWidth)/m_Character->GetuWindowsWidth()*100;
	float porcentajeHeight = (m_Character->GetPosition().y+(position_percent.y/100*windowsHeight)/m_Character->GetuWindowsHeight()*100);
	m_Character->SetPositionPercent(Math::Vect2f(50, 50));
	m_Character->SetTexture(TextureMInstance->GetTexture("data/textures/nova.png"), "character");
	m_Character->SetActiveTexture("character");
}

CMap::~CMap()
{
	CHECKED_DELETE( m_Character );
}
//---------------CGuiElement Interface----------------------
void CMap::Render ()
{
   CImage::Render();
   m_Character->Render();
}

void CMap::Update ()
{
  CImage::Update();
  m_Character->Update();
}


