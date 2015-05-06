#include "WWSoundManager.h"

//WWise Includes
#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>                     // Default memory and stream managers
#include <AK/SoundEngine/Common/IAkStreamMgr.h>                 // Streaming Manager
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>                    // Thread defines
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/Plugin/AllPluginsRegistrationHelpers.h>
#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

#include <Windows.h>
#include <string>
#include "XML\XMLTreeNode.h"
#include "Utils/Defines.h"
#include "EngineConfig.h"
#include "AK/SoundEngine/Common/AkTypes.h"

namespace AK
{
#ifdef WIN32

void* AllocHook( size_t in_size )
{
  return malloc( in_size );
}

void FreeHook( void* in_ptr )
{
  free( in_ptr );
}

void* VirtualAllocHook(
  void* in_pMemAddress,
  size_t in_size,
  DWORD in_dwAllocationType,
  DWORD in_dwProtect
)

{
  return VirtualAlloc( in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect );
}

void VirtualFreeHook(
  void* in_pMemAddress,
  size_t in_size,
  DWORD in_dwFreeType
)

{
  VirtualFree( in_pMemAddress, in_size, in_dwFreeType );
}

#endif
}

CWWSoundManager::CWWSoundManager( const CXMLTreeNode& atts )
  : CManager( atts )
{

}

void CWWSoundManager::Done()
{

#ifndef AK_OPTIMIZED
  //
  // Terminate Communication Services
  //
  AK::Comm::Term();
#endif // AK_OPTIMIZED

  AK::MusicEngine::Term();
  AK::SoundEngine::Term();

  m_lowLevelIO->Term();
  delete m_lowLevelIO;

  if ( AK::IAkStreamMgr::Get() )
    AK::IAkStreamMgr::Get()->Destroy();

  AK::MemoryMgr::Term();

  m_GameObjectMap.clear();
}


void CWWSoundManager::Init()
{

  AkMemSettings memSettings;
  memSettings.uMaxNumPools = 20;

  if ( AK::MemoryMgr::Init( &memSettings ) != AK_Success )
  {
    assert( ! "Could not create the memory manager." );
    return;
  }

  AkStreamMgrSettings stmSettings;
  AK::StreamMgr::GetDefaultSettings( stmSettings );

  // Customize the Stream Manager settings here.

  if ( !AK::StreamMgr::Create( stmSettings ) )
  {
    assert( ! "Could not create the Streaming Manager" );
    return;
  }

  AkDeviceSettings deviceSettings;
  AK::StreamMgr::GetDefaultDeviceSettings( deviceSettings );

  // Customize the streaming device settings here.
  m_lowLevelIO = new CAkDefaultIOHookBlocking();


  if ( m_lowLevelIO->Init( deviceSettings ) != AK_Success )
  {
    assert( ! "Could not create the streaming device and Low-Level I/O system" );
    return;
  }

  AkInitSettings initSettings;
  AkPlatformInitSettings platformInitSettings;
  AK::SoundEngine::GetDefaultInitSettings( initSettings );
  AK::SoundEngine::GetDefaultPlatformInitSettings( platformInitSettings );

  if ( AK::SoundEngine::Init( &initSettings, &platformInitSettings ) != AK_Success )
  {
    assert( ! "Could not initialize the Sound Engine." );
    return;
  }

  AkMusicSettings musicInit;
  AK::MusicEngine::GetDefaultInitSettings( musicInit );

  if ( AK::MusicEngine::Init( &musicInit ) != AK_Success )
  {
    assert( ! "Could not initialize the Music Engine." );
    return;
  }

  AK::SoundEngine::RegisterAllPlugins();


#ifndef AK_OPTIMIZED
  //
  // Initialize communications (not in release build!)
  //
  AkCommSettings commSettings;
  AK::Comm::GetDefaultInitSettings( commSettings );

  if ( AK::Comm::Init( commSettings ) != AK_Success )
  {
    assert( ! "Could not initialize communication." );
    return;
  }

#endif // AK_OPTIMIZED

  Load( mConfigPath );
}

void CWWSoundManager::Update()
{
  AK::SoundEngine::RenderAudio();
}

bool CWWSoundManager::Load( const std::string& xmlFile )
{

  CEngineConfig* lEngineConfig = CEngineConfig::GetSingletonPtr();

  m_lowLevelIO->SetBasePath( L"./Data/sounds/" );

  AkBankID bankID;
  m_ObjectId = 2;

  CXMLTreeNode l_File;

  if ( false == l_File.LoadFile( xmlFile.c_str() ) )
  {
    LOG_ERROR_APPLICATION( "CWWSoundManager::Load --> Error loading XML %s.",
                           xmlFile.c_str() );
    exit( EXIT_FAILURE );
  }

  CXMLTreeNode  sounds_TreeNode = l_File["sounds"];

  if ( sounds_TreeNode.Exists() )
  {
    int count = sounds_TreeNode.GetNumChildren();

    for ( int i = 0; i < count; ++i )
    {
      std::string TagName = sounds_TreeNode( i ).GetName();

      if ( TagName == "Bank" )
      {
        std::string StrBankName = std::string( sounds_TreeNode( i ).GetPszProperty( "name", "" ) );

        AKRESULT l_AkResult = AK::SoundEngine::LoadBank( StrBankName.c_str(), AK_DEFAULT_POOL_ID, bankID );

        if ( l_AkResult != AK_Success )
        {
          LOG_ERROR_APPLICATION( "CWWSoundManager::Load --> Error loading bank %s.",
                                 StrBankName.c_str() );
          return false;
        }

      }
      else if ( TagName == "GameObject2d" )
      {

        std::string StrGameObject2dName = std::string( sounds_TreeNode( i ).GetPszProperty( "name", "" ) );
        std::string StrRegisteredGameObject2d = std::string( sounds_TreeNode( i ).GetPszProperty( "register", "" ) );

        m_GameObjectMap[StrGameObject2dName.c_str()] =  ++m_ObjectId;

        if ( StrRegisteredGameObject2d == "true" )
        {
          AKRESULT l_AkResult =  RegisterGameObject( StrGameObject2dName.c_str() );

          if ( l_AkResult != AK_Success )
          {
            LOG_ERROR_APPLICATION( "CWWSoundManager::Load --> Error loading bank %s.",
                                   StrGameObject2dName.c_str() );
            return false;
          }


        }
      }
      else if ( TagName == "GameObject3d" )
      {

        std::string StrGameObject3dName = std::string( sounds_TreeNode( i ).GetPszProperty( "name", "" ) );
        std::string StrRegisteredGameObject3d = std::string( sounds_TreeNode( i ).GetPszProperty( "register", "" ) );

        m_GameObjectMap[StrGameObject3dName.c_str()] =  ++m_ObjectId;

        if ( StrRegisteredGameObject3d == "true" )
        {
          AKRESULT l_AkResult =  RegisterGameObject( StrGameObject3dName.c_str() );

          if ( l_AkResult != AK_Success )
          {
            LOG_ERROR_APPLICATION( "CWWSoundManager::Load --> Error loading bank %s.",
                                   StrGameObject3dName.c_str() );
            return false;
          }
        }

        int l_NumSources = sounds_TreeNode( i ).GetNumChildren();
        std::vector<Math::Vect3f> l_PosList( l_NumSources );
        std::vector<Math::Vect3f> l_OrientationList( l_NumSources ) ;

        if ( l_NumSources > 0 )
        {
          for ( int k = 0; k < l_NumSources; ++k )
          {
            l_PosList[k] = Math::Vect3f( sounds_TreeNode( i )( k ).GetVect3fProperty( "pos", Math::v3fZERO, false ) );
            l_OrientationList[k] = Math::Vect3f( sounds_TreeNode( i )( k ).GetVect3fProperty( "dir", Math::v3fZERO, false ) );
          }

          std::string l_TypeSource = sounds_TreeNode( i ).GetPszISOProperty( "type", "", false );

          SetGameObjectMultiplePositions( StrGameObject3dName.c_str(), l_PosList, l_OrientationList, l_TypeSource );

        }
        else
        {
          Math::Vect3f PosGameObject3d = Math::Vect3f( sounds_TreeNode( i ).GetVect3fProperty( "pos", Math::Vect3f( 0.0f, 0.0f, 0.0f ).GetNormalized() ) );
          Math::Vect3f DirGameObject3d = Math::Vect3f( sounds_TreeNode( i ).GetVect3fProperty( "dir", Math::Vect3f( 0.0f, 0.0f, 0.0f ).GetNormalized() ) );

          SetGameObjectPosition( StrGameObject3dName.c_str(), PosGameObject3d, DirGameObject3d );
        }
      }

      if ( TagName == "InitEvent" )
      {

        std::string l_name = std::string( sounds_TreeNode( i ).GetPszProperty( "name", "" ) );
        std::string l_gameObject = std::string( sounds_TreeNode( i ).GetPszProperty( "gameObject", "" ) );

        PlayEvent( l_name, l_gameObject );

      }

    }
  }

  return true;
}

AKRESULT CWWSoundManager::SetSwitch( std::string _Group, std::string _Gravel, std::string _KeyGameObjectMap )
{
  return AK::SoundEngine::SetSwitch( _Group.c_str(), _Gravel.c_str(), m_GameObjectMap[_KeyGameObjectMap] );
}

AKRESULT CWWSoundManager::RegisterGameObject( std::string _KeyGameObjectMap )
{
  return AK::SoundEngine::RegisterGameObj( m_GameObjectMap[_KeyGameObjectMap], _KeyGameObjectMap.c_str() );
}

AKRESULT CWWSoundManager::UnregisterGameObject( std::string _KeyGameObjectMap )
{
  return AK::SoundEngine::UnregisterGameObj( m_GameObjectMap[_KeyGameObjectMap] );
}

AKRESULT CWWSoundManager::SetListenerPosition( Math::Vect3f _ListenerPosition, Math::Vect3f _Direction, Math::Vect3f _Up )
{
  AkListenerPosition l_ListenerPosition;

  l_ListenerPosition.Position.X = _ListenerPosition.x;
  l_ListenerPosition.Position.Y = _ListenerPosition.y;
  l_ListenerPosition.Position.Z = _ListenerPosition.z;

  Math::Vect3f l_DirectionNorm  =   _Direction.GetNormalized();
  Math::Vect3f l_UpNorm         =   _Up.GetNormalized();

  l_ListenerPosition.OrientationFront.X = l_DirectionNorm.x;
  l_ListenerPosition.OrientationFront.Y = l_DirectionNorm.y;
  l_ListenerPosition.OrientationFront.Z = l_DirectionNorm.z;

  l_ListenerPosition.OrientationTop.X = l_UpNorm.x;
  l_ListenerPosition.OrientationTop.Y = l_UpNorm.y;
  l_ListenerPosition.OrientationTop.Z = l_UpNorm.z;

  return AK::SoundEngine::SetListenerPosition( l_ListenerPosition );
}

AKRESULT CWWSoundManager::SetGameObjectPosition( std::string _KeyGameObjectMap, Math::Vect3f _GameObjectPosition,
    Math::Vect3f _GameObjectOrientation )
{
  AkSoundPosition l_AKGameObjectPosition;

  l_AKGameObjectPosition.Position.X = _GameObjectPosition.x;
  l_AKGameObjectPosition.Position.Y = _GameObjectPosition.y;
  l_AKGameObjectPosition.Position.Z = _GameObjectPosition.z;

  Math::Vect3f l_GameObjectPositionNorm = _GameObjectPosition.GetNormalized();

  l_AKGameObjectPosition.Orientation.X = l_GameObjectPositionNorm.x;
  l_AKGameObjectPosition.Orientation.Y = l_GameObjectPositionNorm.y;
  l_AKGameObjectPosition.Orientation.Z = l_GameObjectPositionNorm.z;


  return AK::SoundEngine::SetPosition( m_GameObjectMap[_KeyGameObjectMap] , l_AKGameObjectPosition );
}

AKRESULT CWWSoundManager::SetGameObjectMultiplePositions( std::string _KeyGameObjectMap, std::vector<Math::Vect3f> _GameObjectPosition,
    std::vector<Math::Vect3f> _GameObjectOrientation, std::string _TypePos )
{

  int l_NumPositions = _GameObjectPosition.size();

  AK::SoundEngine::MultiPositionType l_eMultiPositionType = AK::SoundEngine::MultiPositionType_MultiSources;
  AkSoundPosition* l_AKGameObjectPositions = new( std::nothrow ) AkSoundPosition[l_NumPositions];

  for ( int i = 0; i < l_NumPositions; ++i )
  {
    l_AKGameObjectPositions[i].Position.X = _GameObjectPosition[i].x;
    l_AKGameObjectPositions[i].Position.Y = _GameObjectPosition[i].y;
    l_AKGameObjectPositions[i].Position.Z = _GameObjectPosition[i].z;

    Math::Vect3f l_GameObjectPositionNorm = _GameObjectPosition[i].GetNormalized();

    l_AKGameObjectPositions[i].Orientation.X = l_GameObjectPositionNorm.x;
    l_AKGameObjectPositions[i].Orientation.Y = l_GameObjectPositionNorm.y;
    l_AKGameObjectPositions[i].Orientation.Z = l_GameObjectPositionNorm.z;
  }

  if ( _TypePos == "MultiSource" )
    l_eMultiPositionType = AK::SoundEngine::MultiPositionType_MultiSources;
  else
    l_eMultiPositionType = AK::SoundEngine::MultiPositionType_MultiDirections;

  return AK::SoundEngine::SetMultiplePositions( m_GameObjectMap[_KeyGameObjectMap] , l_AKGameObjectPositions, l_NumPositions, l_eMultiPositionType );
}

AKRESULT CWWSoundManager::SetState( std::string _Group, std::string _State )
{
  return  AK::SoundEngine::SetState( _Group.c_str(), _State.c_str() );
}

AkPlayingID CWWSoundManager::PlayEvent( std::string _Event,  std::string _KeyGameObjectMap )
{
  return AK::SoundEngine::PostEvent( _Event.c_str(), m_GameObjectMap[_KeyGameObjectMap] );
}

void CWWSoundManager::PauseEvent( std::string _Event,  std::string _KeyGameObjectMap )
{
  if ( AK::SoundEngine::IsInitialized() )
  {
    AK::SoundEngine::PostEvent( _Event.c_str(), m_GameObjectMap[_KeyGameObjectMap] );
    AK::SoundEngine::RenderAudio();

    AK::SoundEngine::Suspend( false );
    SetGamePaused( true );
  }
}

void CWWSoundManager::ResumeEvent( std::string _Event,  std::string _KeyGameObjectMap )
{
  if ( AK::SoundEngine::IsInitialized() )
  {
    AK::SoundEngine::WakeupFromSuspend();

    AK::SoundEngine::PostEvent( _Event.c_str(), m_GameObjectMap[_KeyGameObjectMap] );
    AK::SoundEngine::RenderAudio();

    SetGamePaused( false );
  }
}


void CWWSoundManager::PauseAllFromGameObject( std::string _KeyGameObjectMap )
{
  if ( AK::SoundEngine::IsInitialized() )
  {
    AK::SoundEngine::PostEvent( "PauseAll", m_GameObjectMap[_KeyGameObjectMap] );
    AK::SoundEngine::RenderAudio();

    AK::SoundEngine::Suspend( false );
    SetGamePaused( true );
  }
}

void CWWSoundManager::ResumeAllFromGameObject( std::string _KeyGameObjectMap )
{
  if ( AK::SoundEngine::IsInitialized() )
  {
    AK::SoundEngine::WakeupFromSuspend();

    AK::SoundEngine::PostEvent( "ResumeAll", m_GameObjectMap[_KeyGameObjectMap] );
    AK::SoundEngine::RenderAudio();

    SetGamePaused( false );
  }
}

AkGameObjectID CWWSoundManager::GetGameObjectMapById( std::string _KeyGameObjectMap )
{
  return m_GameObjectMap[_KeyGameObjectMap];
}

void CWWSoundManager::SetGameObjectMapById( std::string _KeyGameObjectMap )
{
  m_GameObjectMap[_KeyGameObjectMap] =  ++m_ObjectId;
}


