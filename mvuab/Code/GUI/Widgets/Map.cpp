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
#include <sstream>


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
  m_Player = 0;
  m_Marco = TextureMInstance->GetTexture( Marco );
  m_MinMaxMap[0] = pos_0_0;
  m_MinMaxMap[1] = pos_1_1;
  const std::string& bla( "Data/textures/GUI/Textures_Test/conotrans.png" );
  m_Cone = TextureMInstance->GetTexture( bla );
}

CMap::~CMap()
{
  CHECKED_DELETE( m_Player );

  std::vector<CItemMap*>::iterator it = m_vItems.begin(),
                                   it_end = m_vItems.end();

  for ( ; it != it_end ; ++it )
    CHECKED_DELETE( *it );

  std::vector<CEnemyMap*>::iterator it2 = m_vEnemy.begin(),
                                    it_end2 = m_vEnemy.end();

  for ( ; it2 != it_end2 ; ++it2 )
    CHECKED_DELETE( *it2 );
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

    float l_PosX0 = m_posNPlayer.x - ( m_Width_Map / 2 );
    float l_PosY0 = m_posNPlayer.y - ( m_Height_Map / 2 );
    float l_PosX1 = m_posNPlayer.x + ( m_Width_Map / 2 );
    float l_PosY1 = m_posNPlayer.y + ( m_Height_Map / 2 );

    //RENDER MAPA
    GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_Mapa, l_PosX0,
                                  l_PosY0, l_PosX1, l_PosY1 );

    //RENDER MARCO
    GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_Marco, 0, 0, 1,
                                  1 );

    //RENDER ITEM
    for ( size_t i = 0 ; i < m_vItems.size() ; ++i )
    {
      if ( m_vItems[i]->m_PosInMap.x > l_PosX0 && m_vItems[i]->m_PosInMap.x < l_PosX1 )
      {
        if ( m_vItems[i]->m_PosInMap.y > l_PosY0  && m_vItems[i]->m_PosInMap.y < l_PosY1 )
        {
          Math::Vect2i l_drawPos = m_Position + Math::Vect2i( ( uint32 )( ( m_vItems[i]->m_PosInMap.x - l_PosX0 ) / m_Width_Map * m_uWidth ),
                                   ( uint32 )( ( m_vItems[i]->m_PosInMap.y - l_PosY0 ) / m_Height_Map * m_uHeight ) );
          GraphicsInstance->DrawQuad2D( l_drawPos, m_vItems[i]->m_Width, m_vItems[i]->m_Height, CGraphicsManager::CENTER,
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
                                    -m_Player->m_Yaw, CGraphicsManager::CENTER,
                                    m_Player->m_Texture );
    }

    //RENDER ENEMYS
    for ( size_t i = 0 ; i < m_vEnemy.size() ; ++i )
    {
      if ( m_vEnemy[i]->m_PosInMap.x > l_PosX0 && m_vEnemy[i]->m_PosInMap.x < l_PosX1 )
      {
        if ( m_vEnemy[i]->m_PosInMap.y > l_PosY0 && m_vEnemy[i]->m_PosInMap.y < l_PosY1 )
        {
          Math::Vect2i l_drawPos = m_Position + Math::Vect2i( ( uint32 )( ( m_vEnemy[i]->m_PosInMap.x - l_PosX0 ) / m_Width_Map * m_uWidth ),
                                   ( uint32 )( ( m_vEnemy[i]->m_PosInMap.y - l_PosY0 ) / m_Height_Map * m_uHeight ) );
          //l_drawPos.x = l_drawPos.x + m_vEnemy[i]->m_Width/2;
          GraphicsInstance->DrawQuad2D( l_drawPos, m_vEnemy[i]->m_Width, m_vEnemy[i]->m_Height, -m_vEnemy[i]->m_Yaw, CGraphicsManager::CENTER,
                                        m_vEnemy[i]->m_Texture );
          /*Math::Vect2f up(cos(m_vEnemy[i]->m_Yaw), sin(m_vEnemy[i]->m_Yaw));
              Math::Vect2i int_UP = Math::Vect2i((int)(up.x*-17),(int)(up.y*-17));
          GraphicsInstance->DrawQuad2D(l_drawPos+int_UP, 40,40, m_vEnemy[i]->m_Yaw, CGraphicsManager::CENTER, m_Cone);*/
        }
      }
    }
  }
}

void CMap::Update()
{
  if ( m_Player )
  {
    ScriptMInstance->RunCode( m_Player->m_PositionScript );
    ScriptMInstance->RunCode( m_Player->m_OrientationScript );
  }

  for ( size_t i = 0 ; i < m_vEnemy.size() ; ++i )
  {
    ScriptMInstance->RunCode( m_vEnemy[i]->m_PositionScript );
    ScriptMInstance->RunCode( m_vEnemy[i]->m_OrientationScript );
  }

  if ( CGuiElement::m_bIsVisible && CGuiElement::m_bIsActive )
  {
    m_posNPlayer = NormalizePlayerPos( m_Player->m_PosPlayer.x, m_Player->m_PosPlayer.z );

    for ( size_t i = 0 ; i < m_vEnemy.size() ; ++i )
      m_vEnemy[i]->m_PosInMap = NormalizePlayerPos( m_vEnemy[i]->m_PosIn3D.x, m_vEnemy[i]->m_PosIn3D.z );
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

void CMap::AddItem( const std::string& Name, const std::string& Texture, Math::Vect3f PosInMap3D, uint32 Width,
                    uint32 Height, float Yaw, std::string  PositionScript, std::string  OrientationScript )
{
  Math::Vect2f l_posInMap = NormalizePlayerPos( PosInMap3D.x, PosInMap3D.z );
  CItemMap* l_ItemMap     = new CItemMap( Name, TextureMInstance->GetTexture( Texture ), l_posInMap, Width, Height, Yaw, PositionScript,
                                          OrientationScript );
  m_vItems.push_back( l_ItemMap );
}

void CMap::AddEnemy( const std::string& Name, const std::string& Texture, uint32 Width,
                     uint32 Height, float Yaw, std::string  PositionScript, std::string  OrientationScript )
{
  //if ( EnemyMInstance != NULL )
  //{
  //  CEnemy* l_Enemy = EnemyMInstance->GetResource( Name );

  //  if ( l_Enemy )
  //  {
  //    Math::Vect3f PosInMap3d = l_Enemy->GetPosition();
  //    Math::Vect2f l_posInMap = NormalizePlayerPos( PosInMap3d.x, PosInMap3d.z );
  //    CEnemyMap* l_EnemyMap   = new CEnemyMap( Name, TextureMInstance->GetTexture( Texture ), PosInMap3d, l_posInMap, Width,
  //        Height, Yaw, PositionScript, OrientationScript );
  //    m_vEnemy.push_back( l_EnemyMap );
  //  }
  //}
}

void CMap::AddEnemys( const std::string& Texture, uint32 Width, uint32 Height, std::string  PositionScript, std::string  OrientationScript )
{
  /*if ( EnemyMInstance != NULL )
  {
    std::map<std::string, CEnemy*> l_MapEnemy         = EnemyMInstance->GetResourcesMap();

    std::map<std::string, CEnemy*>::iterator  it      = l_MapEnemy.begin(),
                                              it_end  = l_MapEnemy.end();

    for ( ; it != it_end ; ++it )
    {
      Math::Vect3f PosInMap3d = it->second->GetPosition();
      Math::Vect2f l_posInMap = NormalizePlayerPos( PosInMap3d.x, PosInMap3d.z );
      float        lYaw       = it->second->GetYaw();
      std::stringstream PositionScriptComplete;
      std::stringstream OrientationScriptComplete;
      PositionScriptComplete    << PositionScript     << "('" << it->first << "')";
      OrientationScriptComplete << OrientationScript  << "('" << it->first << "')";
      CEnemyMap* l_EnemyMap = new CEnemyMap( it->first, TextureMInstance->GetTexture( Texture ), PosInMap3d, l_posInMap, Width,
                                             Height, lYaw, PositionScriptComplete.str(), OrientationScriptComplete.str() );
      m_vEnemy.push_back( l_EnemyMap );
    }
  }*/
  std::stringstream parametros;
  parametros << Texture << "', '" << Width << "', '" << Height << "', '" << PositionScript << "', '" << OrientationScript << "')";
  ScriptMInstance->RunCode( "AddEnemys('" + parametros.str() );
}

void CMap::AddEnemyLUA( const std::string& aName, CTexture* aTexture, Math::Vect3f aPosInMap3d, Math::Vect2f aPosInMap, uint32 aWidth,
                        uint32 aHeight, float aYaw, std::string  aPositionScript, std::string  aOrientationScript )
{
  CEnemyMap* l_EnemyMap = new CEnemyMap( aName, aTexture, aPosInMap3d, aPosInMap, aWidth, aHeight, aYaw, aPositionScript, aOrientationScript );
  m_vEnemy.push_back( l_EnemyMap );
}

void CMap::AddPlayer( const std::string& Name, const std::string& Texture, Math::Vect3f PosPlayer, uint32 Width, uint32 Height, float Yaw,
                      std::string  PositionScript, std::string  OrientationScript )
{
  m_Player = new CPlayer( Name, TextureMInstance->GetTexture( Texture ), PosPlayer, Width, Height, Yaw, PositionScript, OrientationScript );
}

CMap::CItemMap::CItemMap( std::string Name, CTexture* Texture, Math::Vect2f PosInMap, uint32 Width, uint32 Height, float Yaw,
                          std::string  PositionScript, std::string  OrientationScript )
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

CMap::CPlayer::CPlayer( std::string Name, CTexture* Texture, Math::Vect3f PosPlayer, uint32 Width, uint32 Height, float Yaw,
                        std::string  PositionScript, std::string  OrientationScript )
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

CMap::CEnemyMap::CEnemyMap( std::string Name, CTexture* Texture, Math::Vect3f PosIn3D, Math::Vect2f PosInMap, uint32 Width, uint32 Height, float Yaw,
                            std::string  PositionScript, std::string  OrientationScript )
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
  m_Player->m_PosPlayer = position;//Math::Vect2f( position.x, position.z );
}

void CMap::SetYawPlayer( float Yaw )
{
  m_Player->m_Yaw = Yaw;
}

void CMap::SetPositionEnemy( const std::string& Name,  Math::Vect3f position )
{
  for ( size_t i = 0 ; i < m_vEnemy.size() ; ++i )
  {
    if ( m_vEnemy[i]->m_Name == Name )
    {
      m_vEnemy[i]->m_PosIn3D = position;//Math::Vect2f( position.x, position.z );
      return;
    }
  }
}

void CMap::SetYawEnemy( const std::string& Name, float Yaw )
{
  for ( size_t i = 0 ; i < m_vEnemy.size() ; ++i )
  {
    if ( m_vEnemy[i]->m_Name == Name )
    {
      m_vEnemy[i]->m_Yaw = -Yaw + Math::half_pi32;
      return;
    }
  }
}
