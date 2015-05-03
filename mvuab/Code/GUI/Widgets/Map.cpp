#include "Widgets\Map.h"
#include "EngineManagers.h"
#include "Texture\TextureManager.h"
#include "Texture\Texture.h"
#include "Texture\GUITexture.h"
#include "GraphicsManager.h"
#include "Cameras\CameraManager.h"
#include "Object3D.h"

//---Constructor
CMap::CMap( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
            const Math::Vect2f position_percent, std::string lit, uint32 textHeightOffset, uint32 textWidthOffset,
            bool isVisible, bool isActive )
  : CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, IMAGE, lit, textHeightOffset, textWidthOffset, isVisible,
                 isActive )
{
  m_Mapa = TextureMInstance->GetTexture( "Data/textures/mapa.png" );
  m_Player = TextureMInstance->GetTexture( "Data/textures/GUI/Textures_Test/flecha.png" );
  m_Camara = TextureMInstance->GetTexture( "Data/textures/GUI/Textures_Test/punto.png" );
  m_Marco = TextureMInstance->GetTexture( "Data/textures/GUI/Textures_Test/marco2.png" );
  //m_SizeWorld3D = Math::Vect2f( 100.f, 100.f ); // Unidades totales del grid (mundo 3D) en X & Z
}

CMap::~CMap()
{
}
//---------------CGuiElement Interface----------------------
void CMap::Render()
{
  if ( CGuiElement::m_bIsVisible )
  {
    //Primero renderizamos todos los hijos que pudiera tener el Button:
    CGuiElement::Render();

    //Renderizamos el fondo del mapa o radar

    //Math::Vect2i pos = CGuiElement::m_Position;
    // Cálculo posición del Player en píxeles de mapa según su size & posición normalizada
    float deltaX = m_posNPlayer.x * ( float )m_uWidth;
    float deltaY = m_posNPlayer.y * ( float )m_uHeight;
    // Posición de pintado = posición de pintado del mapa en píxeles de pantalla + deltaXY
    Math::Vect2i l_drawPos = m_Position + Math::Vect2i( m_uWidth / 2, m_uHeight / 2 );
    //Renderizamos la posición del player
    Math::Vect2f up( cos( CameraMInstance->GetCurrentCamera()->GetYaw() ), sin( CameraMInstance->GetCurrentCamera()->GetYaw() ) );
    Math::Vect2i int_UP = Math::Vect2i( ( int )( up.x * -17 ), ( int )( up.y * -17 ) );

    //GraphicsInstance->DrawQuad2D( l_drawPos + int_UP, 40, 40, CameraMInstance->GetCurrentCamera()->GetYaw(), CGraphicsManager::CENTER, m_Camara );
    if ( m_posNPlayer.x <= 0.15f )
      m_posNPlayer.x = 0.15f;

    if ( m_posNPlayer.x >= 0.85f )
      m_posNPlayer.x = 0.85f;

    if ( m_posNPlayer.y <= 0.1f )
      m_posNPlayer.y = 0.1f;

    if ( m_posNPlayer.y >= 0.9f )
      m_posNPlayer.y = 0.9f;

    GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_Mapa,
                                  m_posNPlayer.x - 0.15f, m_posNPlayer.y - 0.1f, m_posNPlayer.x + 0.15f, m_posNPlayer.y + 0.1f );

    GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_Marco,
                                  0, 0, 1, 1 );

    GraphicsInstance->DrawQuad2D( l_drawPos, 20, 20, CameraMInstance->GetCurrentCamera()->GetYaw() + Math::PI_32_VALUE, CGraphicsManager::CENTER,
                                  m_Player );


    //Pintamos el boton del slider
    //  m_Button.Render(GraphicsManager, fm);
  }
}

void CMap::Update()
{
  if ( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
  {
    m_posNPlayer = NormalizePlayerPos( CameraMInstance->GetCurrentCamera()->GetPosition().x,
                                       CameraMInstance->GetCurrentCamera()->GetPosition().z );
  }
}

Math::Vect2f CMap::NormalizePlayerPos( float x, float z )
{
  // Mapeo de [-50,50] 3D a [0,1] Mapa/radar
  Math::Vect2f pos;
  pos.y = 1 - ( ( x + 50 ) / 100 );
  pos.x = 1 - ( ( z + 50 ) / 100 );

  // Control del player respecto a los límites del mapa/radar
  if ( pos.x < 0.f ) pos.x = 0.f;
  else if ( pos.x > 1.f ) pos.x = 1.f;

  if ( pos.y < 0.f ) pos.y = 0.f;
  else if ( pos.y > 1.f ) pos.y = 1.f;

  return pos;
}

void  CMap::OnClickedChild( const std::string& name )
{
}