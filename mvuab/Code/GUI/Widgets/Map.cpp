#include "Widgets\Map.h"
#include "EngineManagers.h"
#include "Texture\TextureManager.h"
#include "Texture\Texture.h"
#include "Texture\GUITexture.h"
#include "GraphicsManager.h"
#include "Cameras\CameraManager.h"
#include "Object3D.h"

//---Constructor
CMap::CMap( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent, const Math::Vect2f position_percent,
            const std::string& Marco, const std::string& Map, const Math::Vect2f pos_0_0, const Math::Vect2f pos_1_1, float h_map, float w_map, bool isVisible,
            bool isActive )
  : CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, IMAGE, "", 2U, 2U, isVisible,
                 isActive )
  , m_Width_Map( w_map )
  , m_Height_Map( h_map )
{
  m_Mapa = TextureMInstance->GetTexture( Map );
  m_Player = TextureMInstance->GetTexture( "Data/textures/GUI/Textures_Test/flecha.png" );
  //m_Camara = TextureMInstance->GetTexture( "Data/textures/GUI/Textures_Test/punto.png" );
  m_Marco = TextureMInstance->GetTexture( Marco );
  m_MinMaxMap[0] = pos_0_0;
  m_MinMaxMap[1] = pos_1_1;
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
    if ( m_posNPlayer.x <= ( m_Width_Map / 2 ) )
      m_posNPlayer.x = m_Width_Map / 2;

    if ( m_posNPlayer.x >= ( 1 - ( m_Width_Map / 2 ) ) )
      m_posNPlayer.x = 1 - ( m_Width_Map / 2 );

    if ( m_posNPlayer.y <= m_Height_Map / 2 )
      m_posNPlayer.y = m_Height_Map / 2;

    if ( m_posNPlayer.y >= ( 1 - ( m_Height_Map / 2 ) ) )
      m_posNPlayer.y = 1 - ( m_Height_Map / 2 ) ;

    GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_Mapa,
                                  m_posNPlayer.x - ( m_Width_Map / 2 ), m_posNPlayer.y - ( m_Height_Map / 2 ), m_posNPlayer.x + ( m_Width_Map / 2 ),
                                  m_posNPlayer.y + ( m_Height_Map / 2 ) );

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
  /*pos.y = 1 - ( ( x + 50 ) / 100 );
  pos.x = 1 - ( ( z + 50 ) / 100 );*/

  pos.x = ( ( x - m_MinMaxMap[0].x ) / ( m_MinMaxMap[1].x - m_MinMaxMap[0].x ) );
  pos.y = ( ( z - m_MinMaxMap[0].y ) / ( m_MinMaxMap[1].y - m_MinMaxMap[0].y ) );

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