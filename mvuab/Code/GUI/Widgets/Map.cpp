#include "Widgets\Map.h"
#include "EngineManagers.h"
#include "Texture\TextureManager.h"
#include "Texture\Texture.h"
#include "GraphicsManager.h"
#include "Cameras\CameraManager.h"
#include "Object3D.h"
#include "Characters\Enemies\EnemyManager.h"
#include "ScriptManager.h"
#include <vector>
#include <sstream>

//---Constructor
CMap::CMap( const CXMLTreeNode& aNode, const Math::Vect2i& screenResolution )
  : CGuiElement( aNode, screenResolution )
  , m_Width_Map( aNode.GetAttribute<float>( "width_map", 0.02f ) )
  , m_Height_Map( aNode.GetAttribute<float>( "height_map", 0.02f ) )
  , m_Mapa( aNode.GetAttribute<CTexture>( "texture_map" ) )
 // , m_Marco( aNode.GetAttribute<CTexture>( "texture_marco" ) )
  , m_Cone( aNode.GetAttribute<CTexture>( "Data/textures/GUI/Textures_Test/conotrans.png" ) ) // TODO Ruly, poner esto en el xml tambien
  , m_Player( 0 )
{
  /*AddEnemys
  (
    aNode.GetAttribute<std::string>( "texture_enemy", "" ),
    aNode.GetAttribute<int32>( "width_enemy", 50 ),
    aNode.GetAttribute<int32>( "height_enemy", 50 ),
    aNode.GetAttribute<std::string>( "get_position_script", "" ),
    aNode.GetAttribute<std::string>( "orientation", "" )
  );*/

  m_MinMaxMap[0] = aNode.GetAttribute<Math::Vect2f>( "pos_0_0_3d_map", Math::Vect2f( 0.f,
                   0.f ) );
  m_MinMaxMap[1] = aNode.GetAttribute<Math::Vect2f>( "pos_1_1_3d_map", Math::Vect2f( 0.f,
                   0.f ) );

  for ( int i = 0, count = aNode.GetNumChildren(); i < count; ++i )
  {
    const CXMLTreeNode& pSubNewNode       = aNode( i );
    const std::string& NameItem           = pSubNewNode.GetAttribute<std::string>( "name", "defaultItemElement" );
    const std::string& TextureItem        = pSubNewNode.GetAttribute<std::string>( "texture", "no_texture" );
    const std::string& position_script    = pSubNewNode.GetAttribute<std::string>( "get_position_script", "no_script" );
    const std::string& orientation_script = pSubNewNode.GetAttribute<std::string>( "orientation", "no_script" );
    uint32 WidthItem                      = pSubNewNode.GetAttribute<uint32>( "width", 50 );
    uint32 HeightItem                     = pSubNewNode.GetAttribute<uint32>( "height", 50 );
    float Yaw                             = pSubNewNode.GetAttribute<float>( "yaw", 0.f );

    const std::string& tagName = pSubNewNode.GetName();
    const Math::Vect3f& pos = pSubNewNode.GetAttribute<Math::Vect3f>( "pos_in_map", Math::Vect3f() );

    if ( tagName == "item" )
      AddItem( NameItem, TextureItem, pos, WidthItem, HeightItem, Yaw, position_script, orientation_script );
    else if ( tagName == "mark_player" )
      AddPlayer( NameItem, TextureItem, pos, WidthItem, HeightItem, Yaw, position_script, orientation_script );
  }
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
    if ( m_posNPlayer.x <= ( m_Width_Map / 4 ) )
      m_posNPlayer.x = m_Width_Map / 4;

    if ( m_posNPlayer.x >= ( 1 - ( m_Width_Map / 4 ) ) )
      m_posNPlayer.x = 1 - ( m_Width_Map / 4 );

    if ( m_posNPlayer.y <= m_Height_Map / 4 )
      m_posNPlayer.y = m_Height_Map / 4;

    if ( m_posNPlayer.y >= ( 1 - ( m_Height_Map / 4 ) ) )
      m_posNPlayer.y = 1 - ( m_Height_Map / 4 ) ;

    float l_PosX0 = m_posNPlayer.x - ( m_Width_Map / 4 );
    float l_PosY0 = m_posNPlayer.y - ( m_Height_Map / 4 );
    float l_PosX1 = m_posNPlayer.x + ( m_Width_Map / 4 );
    float l_PosY1 = m_posNPlayer.y + ( m_Height_Map / 4 );

    //RENDER MAPA
    GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_Mapa, l_PosX0,
                                  l_PosY0, l_PosX1, l_PosY1 );

    //RENDER MARCO
 /*   GraphicsInstance->DrawQuad2D( CGuiElement::m_Position, CGuiElement::m_uWidth, CGuiElement::m_uHeight, CGraphicsManager::UPPER_LEFT, m_Marco, 0, 0, 1,
                                  1 );*/

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

void CMap::AddEnemy( CTexture* aTexture, const std::string& Name, uint32 Width,
                     uint32 Height, float Yaw, std::string  PositionScript, std::string  OrientationScript )
{
  /*if ( EnemyMInstance != NULL )
  {
      CEnemy* l_Enemy = EnemyMInstance->GetResource( Name );

      if ( l_Enemy )
      {
          Math::Vect3f PosInMap3d = l_Enemy->GetPosition();
          Math::Vect2f l_posInMap = NormalizePlayerPos( PosInMap3d.x, PosInMap3d.z );
          CEnemyMap* l_EnemyMap   = new CEnemyMap( Name, aTexture, PosInMap3d, l_posInMap, Width,
                  Height, Yaw, PositionScript, OrientationScript );
          m_vEnemy.push_back( l_EnemyMap );
      }
  }*/
}

void CMap::AddEnemys( const std::string& aTexture, uint32 Width, uint32 Height, std::string  PositionScript, std::string  OrientationScript )
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
          CEnemyMap* l_EnemyMap = new CEnemyMap( it->first, aTexture, PosInMap3d, l_posInMap, Width,
                                                 Height, lYaw, PositionScriptComplete.str(), OrientationScriptComplete.str() );
          m_vEnemy.push_back( l_EnemyMap );
      }
  }*/
  std::stringstream parametros;
  parametros << aTexture << "', '" << Width << "', '" << Height << "', '" << PositionScript << "', '" << OrientationScript << "')";
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