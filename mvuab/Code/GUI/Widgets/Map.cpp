#include "Widgets\Map.h"
#include "EngineManagers.h"
#include "Texture\TextureManager.h"
#include "Texture\Texture.h"
#include "Texture\GUITexture.h"
#include "GraphicsManager.h"
#include "Cameras\CameraManager.h"
#include "Object3D.h"
#include <vector>

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
  m_Player = 0;//TextureMInstance->GetTexture( "Data/textures/GUI/Textures_Test/flecha.png" );
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
    //Primero renderizamos todos los hijos que pudiera tener el Mapa:
    CGuiElement::Render();

    //Nos aseguramos que la posición del player no salga del mapa
    if ( m_posNPlayer.x <= ( m_Width_Map / 2 ) )
      m_posNPlayer.x = m_Width_Map / 2;

    if ( m_posNPlayer.x >= ( 1 - ( m_Width_Map / 2 ) ) )
      m_posNPlayer.x = 1 - ( m_Width_Map / 2 );

    if ( m_posNPlayer.y <= m_Height_Map / 2 )
      m_posNPlayer.y = m_Height_Map / 2;

    if ( m_posNPlayer.y >= ( 1 - ( m_Height_Map / 2 ) ) )
      m_posNPlayer.y = 1 - ( m_Height_Map / 2 ) ;

    //RENDER MAPA
    GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_Mapa,
                                  m_posNPlayer.x - ( m_Width_Map / 2 ), m_posNPlayer.y - ( m_Height_Map / 2 ), m_posNPlayer.x + ( m_Width_Map / 2 ),
                                  m_posNPlayer.y + ( m_Height_Map / 2 ) );

    //RENDER MARCO
    GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_Marco,
                                  0, 0, 1, 1 );



    //RENDER ITEM
    for ( size_t i = 0 ; i < m_vItems.size() ; ++i )
    {
      if ( ( m_vItems[i]->m_PosInMap.x > ( m_posNPlayer.x - ( m_Width_Map / 2 ) ) ) &&
           ( m_vItems[i]->m_PosInMap.x < ( m_posNPlayer.x + ( m_Width_Map / 2 ) ) ) )
      {
        if ( ( m_vItems[i]->m_PosInMap.y > ( m_posNPlayer.y - ( m_Height_Map / 2 ) ) ) &&
             ( m_vItems[i]->m_PosInMap.y < ( m_posNPlayer.y + ( m_Height_Map / 2 ) ) ) )
        {
          Math::Vect2i l_drawPos = m_Position + Math::Vect2i( ( uint32 )( ( m_vItems[i]->m_PosInMap.x - ( m_posNPlayer.x -
                                   ( m_Width_Map / 2 ) ) ) / m_Width_Map * m_uWidth ),
                                   ( uint32 )( ( m_vItems[i]->m_PosInMap.y - ( m_posNPlayer.y - ( m_Height_Map / 2 ) ) ) / m_Height_Map * m_uHeight ) );
          GraphicsInstance->DrawQuad2D( l_drawPos, m_vItems[i]->m_Width, m_vItems[i]->m_Height, CGraphicsManager::CENTER, m_vItems[i]->m_Texture );
        }
      }
    }

    //RENDER PLAYER
    if ( m_Player )
    {
      // Posición de pintado = posición de pintado del mapa en píxeles de pantalla
      Math::Vect2i l_drawPos = m_Position + Math::Vect2i( m_uWidth / 2, m_uHeight / 2 );
      GraphicsInstance->DrawQuad2D( l_drawPos, m_Player->m_Width, m_Player->m_Height, CameraMInstance->GetCurrentCamera()->GetYaw(),
                                    CGraphicsManager::CENTER,
                                    m_Player->m_Texture );
    }
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
  Math::Vect2f pos;
  //Calculo de la posición del player en el mapa
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

void CMap::AddItem( const std::string& Name, const std::string& Texture, Math::Vect2f PosInMap3d, uint32 Width, uint32 Height )
{
  Math::Vect2f l_posInMap = NormalizePlayerPos( PosInMap3d.x, PosInMap3d.y );
  CItemMap* l_ItemMap = new CItemMap( Name, TextureMInstance->GetTexture( Texture ), l_posInMap, Width, Height );
  m_vItems.push_back( l_ItemMap );
}

void CMap::AddPlayer( const std::string& Name, const std::string& Texture, uint32 Width, uint32 Height )
{
  m_Player = new CPlayer( Name, TextureMInstance->GetTexture( Texture ), Width, Height );
}

CMap::CItemMap::CItemMap( std::string Name, CTexture* Texture, Math::Vect2f PosInMap, uint32 Width, uint32 Height )
{
  m_Name = Name;
  m_Texture = Texture;
  m_PosInMap = PosInMap;
  m_Width = Width;
  m_Height = Height;
}

CMap::CPlayer::CPlayer( std::string Name, CTexture* Texture, uint32 Width, uint32 Height )
{
  m_Name = Name;
  m_Texture = Texture;
  m_Width = Width;
  m_Height = Height;
}