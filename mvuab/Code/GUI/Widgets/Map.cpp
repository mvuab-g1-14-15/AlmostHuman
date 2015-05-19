#include "Widgets\Map.h"
#include "EngineManagers.h"
#include "Texture\TextureManager.h"
#include "Texture\Texture.h"
#include "Texture\GUITexture.h"
#include "GraphicsManager.h"
#include "Cameras\CameraManager.h"
#include "Object3D.h"
#include "Characters\Enemies\EnemyManager.h"
#include "ScriptManager.h"
#include <vector>

//---Constructor
CMap::CMap( uint32 windowsHeight, uint32 windowsWidth, float height_precent, float witdh_percent,
            const Math::Vect2f position_percent,
            const std::string& Marco, const std::string& Map, const Math::Vect2f pos_0_0, const Math::Vect2f pos_1_1, float h_map,
            float w_map, bool isVisible,
            bool isActive )
  : CGuiElement( windowsHeight, windowsWidth, height_precent, witdh_percent, position_percent, IMAGE, "", 2U, 2U,
                 isVisible,
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
    GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight,
                                  CGraphicsManager::UPPER_LEFT, m_Mapa,
                                  m_posNPlayer.x - ( m_Width_Map / 2 ), m_posNPlayer.y - ( m_Height_Map / 2 ), m_posNPlayer.x + ( m_Width_Map / 2 ),
                                  m_posNPlayer.y + ( m_Height_Map / 2 ) );

    //RENDER MARCO
    GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight,
                                  CGraphicsManager::UPPER_LEFT, m_Marco,
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
          GraphicsInstance->DrawQuad2D( l_drawPos, m_vItems[i]->m_Width, m_vItems[i]->m_Height, m_vItems[i]->m_Yaw, CGraphicsManager::UPPER_LEFT,
                                        m_vItems[i]->m_Texture );
        }
      }
    }

    //RENDER PLAYER
    if ( m_Player )
    {
      // Posición de pintado = posición de pintado del mapa en píxeles de pantalla
      Math::Vect2i l_drawPos = m_Position + Math::Vect2i( m_uWidth / 2, m_uHeight / 2 );
      GraphicsInstance->DrawQuad2D( l_drawPos, m_Player->m_Width, m_Player->m_Height,
                                    m_Player->m_Yaw, CGraphicsManager::CENTER,
                                    m_Player->m_Texture );
    }

	//RENDER ENEMYS
    for ( size_t i = 0 ; i < m_vEnemy.size() ; ++i )
    {
      if ( ( m_vEnemy[i]->m_PosInMap.x > ( m_posNPlayer.x - ( m_Width_Map / 2 ) ) ) &&
           ( m_vEnemy[i]->m_PosInMap.x < ( m_posNPlayer.x + ( m_Width_Map / 2 ) ) ) )
      {
        if ( ( m_vEnemy[i]->m_PosInMap.y > ( m_posNPlayer.y - ( m_Height_Map / 2 ) ) ) &&
             ( m_vEnemy[i]->m_PosInMap.y < ( m_posNPlayer.y + ( m_Height_Map / 2 ) ) ) )
        {
          Math::Vect2i l_drawPos = m_Position + Math::Vect2i( ( uint32 )( ( m_vEnemy[i]->m_PosInMap.x - ( m_posNPlayer.x -
                                   ( m_Width_Map / 2 ) ) ) / m_Width_Map * m_uWidth ),
                                   ( uint32 )( ( m_vEnemy[i]->m_PosInMap.y - ( m_posNPlayer.y - ( m_Height_Map / 2 ) ) ) / m_Height_Map * m_uHeight ) );
		  GraphicsInstance->DrawQuad2D( l_drawPos, m_vEnemy[i]->m_Width, m_vEnemy[i]->m_Height, m_vEnemy[i]->m_Yaw, CGraphicsManager::UPPER_LEFT,
                                        m_vEnemy[i]->m_Texture );
        }
      }
    }


  }
}

void CMap::Update()
{
	if(m_Player)
	{
		ScriptMInstance->RunCode(m_Player->m_PositionScript);
		ScriptMInstance->RunCode(m_Player->m_OrientationScript);
	}
	for ( size_t i = 0 ; i < m_vEnemy.size() ; ++i )
    {
		ScriptMInstance->RunCode(m_vEnemy[i]->m_PositionScript);
		ScriptMInstance->RunCode(m_vEnemy[i]->m_OrientationScript);
	}

  if ( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
  {
    m_posNPlayer = NormalizePlayerPos( m_Player->m_PosPlayer.x, m_Player->m_PosPlayer.y );

    for ( size_t i = 0 ; i < m_vEnemy.size() ; ++i )
    {
        m_vEnemy[i]->m_PosInMap = NormalizePlayerPos( m_vEnemy[i]->m_PosIn3D.x, m_vEnemy[i]->m_PosIn3D.y );
    }
  }
}

Math::Vect2f CMap::NormalizePlayerPos( float x, float z )
{
  Math::Vect2f pos;
  //Calculo de la posición del player en el mapa
  pos.x = ( ( x - m_MinMaxMap[0].x ) / ( m_MinMaxMap[1].x - m_MinMaxMap[0].x ) );
  pos.y = ( ( z - m_MinMaxMap[0].y ) / ( m_MinMaxMap[1].y - m_MinMaxMap[0].y ) );

  // Control del player respecto a los límites del mapa/radar
  if ( pos.x < 0.f )
    pos.x = 0.f;
  else if ( pos.x > 1.f )
    pos.x = 1.f;

  if ( pos.y < 0.f )
    pos.y = 0.f;
  else if ( pos.y > 1.f )
    pos.y = 1.f;

  return pos;
}

void CMap::OnClickedChild( const std::string& name )
{

}

void CMap::AddItem( const std::string& Name, const std::string& Texture, Math::Vect2f PosInMap3D, uint32 Width,
                uint32 Height, float Yaw, std::string  PositionScript, std::string  OrientationScript )
{
  Math::Vect2f l_posInMap = NormalizePlayerPos( PosInMap3D.x, PosInMap3D.y );
  CItemMap* l_ItemMap = new CItemMap( Name, TextureMInstance->GetTexture( Texture ), l_posInMap, Width, Height, Yaw, PositionScript, OrientationScript );
  m_vItems.push_back( l_ItemMap );
}

void CMap::AddEnemy( const std::string& Name, const std::string& Texture, uint32 Width,
                     uint32 Height, float Yaw, std::string  PositionScript, std::string  OrientationScript )
{
	CEnemy* l_Enemy = EnemyMInstance->GetResource( Name );
	if (l_Enemy)
	{
		Math::Vect3f PosInMap3d = l_Enemy->GetPosition();
		Math::Vect2f l_posInMap = NormalizePlayerPos( PosInMap3d.x, PosInMap3d.z );
		CEnemyMap* l_EnemyMap = new CEnemyMap( Name, TextureMInstance->GetTexture( Texture ), Math::Vect2f( PosInMap3d.x, PosInMap3d.z ), l_posInMap, Width,
												Height, Yaw, PositionScript, OrientationScript );
		m_vEnemy.push_back( l_EnemyMap );
	}
}

void CMap::AddPlayer( const std::string& Name, const std::string& Texture, Math::Vect2f PosPlayer, uint32 Width, uint32 Height, float Yaw, std::string  PositionScript, std::string  OrientationScript )
{
  m_Player = new CPlayer( Name, TextureMInstance->GetTexture( Texture ), PosPlayer, Width, Height, Yaw, PositionScript, OrientationScript );
}

CMap::CItemMap::CItemMap( std::string Name, CTexture* Texture, Math::Vect2f PosInMap, uint32 Width, uint32 Height, float Yaw, std::string  PositionScript, std::string  OrientationScript )
{
  m_Name = Name;
  m_Texture = Texture;
  m_PosInMap = PosInMap;
  m_Width = Width;
  m_Height = Height;
  m_Yaw = Yaw;
  m_PositionScript = PositionScript;
  m_OrientationScript = OrientationScript;
}

CMap::CPlayer::CPlayer( std::string Name, CTexture* Texture, Math::Vect2f PosPlayer, uint32 Width, uint32 Height, float Yaw, std::string  PositionScript, std::string  OrientationScript )
{
  m_Name = Name;
  m_Texture = Texture;
  m_PosPlayer = PosPlayer;
  m_Width = Width;
  m_Height = Height;
  m_Yaw = Yaw;
  m_PositionScript = PositionScript;
  m_OrientationScript = OrientationScript;
}

CMap::CEnemyMap::CEnemyMap( std::string Name, CTexture* Texture, Math::Vect2f PosIn3D, Math::Vect2f PosInMap, uint32 Width, uint32 Height, float Yaw, std::string  PositionScript, std::string  OrientationScript )
{
  m_Name = Name;
  m_Texture = Texture;
  m_PosIn3D = PosIn3D;
  m_PosInMap = PosInMap;
  m_Width = Width;
  m_Height = Height;
  m_Yaw = Yaw;
  m_PositionScript = PositionScript;
  m_OrientationScript = OrientationScript;
}

void CMap::SetPositionPlayer( Math::Vect3f position )
{
	m_Player->m_PosPlayer = Math::Vect2f( position.x, position.z );
}

void CMap::SetYawPlayer( float Yaw )
{
	m_Player->m_Yaw = Yaw;
}

void CMap::SetPositionEnemy( const std::string& Name,  Math::Vect3f position )
{
	for ( size_t i = 0 ; i < m_vEnemy.size() ; ++i )
    {
		if( m_vEnemy[i]->m_Name == Name )
		{
			m_vEnemy[i]->m_PosIn3D = Math::Vect2f( position.x, position.z );
			break;
		}
	}
}

void CMap::SetYawEnemy( const std::string& Name, float Yaw )
{
	for ( size_t i = 0 ; i < m_vEnemy.size() ; ++i )
    {
		if( m_vEnemy[i]->m_Name == Name )
		{
			m_vEnemy[i]->m_Yaw = Yaw;
			break;
		}
	}
}