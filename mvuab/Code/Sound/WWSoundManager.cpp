#include "WWSoundManager.h"
#include "EngineConfig.h"

//WWise Includes
#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>                     // Default memory and stream managers
#include <AK/SoundEngine/Common/IAkStreamMgr.h>                 // Streaming Manager
#include <AK/SoundEngine/Common/AkStreamMgrModule.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>                    // Thread defines
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/MusicEngine/Common/AkMusicEngine.h>
#include <AK/Plugin/AllPluginsRegistrationHelpers.h>

#ifdef _DEBUG
    #ifndef AK_OPTIMIZED
        #include <AK/Comm/AkCommunication.h>
    #endif // AK_OPTIMIZED
#endif

#include <Windows.h>
#include <string>
#include <math.h>
#include "XML\XMLTreeNode.h"
#include "Utils/Defines.h"
#include "EngineConfig.h"
//#include "AK/SoundEngine/Common/AkTypes.h"

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
    // Note: VirtualAllocHook() may be used by I/O pools of the default implementation
    // of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
    // - refer to the Windows SDK documentation for more details). This is NOT mandatory;
    // you may implement it with a simple malloc().
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
    #ifdef _DEBUG
    #ifndef AK_OPTIMIZED
    //
    // Terminate Communication Services
    //
    AK::Comm::Term();
    #endif // AK_OPTIMIZED
    #endif

    AK::MusicEngine::Term();
    AK::SoundEngine::Term();

    m_lowLevelIO->Term();
    //delete m_lowLevelIO;
    CHECKED_DELETE( m_lowLevelIO );

    if ( AK::IAkStreamMgr::Get() )
    {
        AK::IAkStreamMgr::Get()->Destroy();
    }

    AK::MemoryMgr::Term();

    m_GameObjectMap.clear();

}


void CWWSoundManager::Init()
{

    //
    // Create and initialize an instance of the default memory manager. Note
    // that you can override the default memory manager with your own. Refer
    // to the SDK documentation for more information.
    //

    AkMemSettings memSettings;
    
    memSettings.uMaxNumPools = 100;
  
    if ( AK::MemoryMgr::Init( &memSettings ) != AK_Success )
    {
        assert( ! "Could not create the memory manager." );
    }

    
    //
    //
    // Create a streaming device with blocking low-level I/O handshaking.
    // Note that you can override the default low-level I/O module with your own. Refer
    // to the SDK documentation for more information.
    //
    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings( deviceSettings );

    //
    // Create and initialize an instance of the default streaming manager. Note
    // that you can override the default streaming manager with your own. Refer
    // to the SDK documentation for more information.

    AkStreamMgrSettings stmSettings;
    AK::StreamMgr::GetDefaultSettings( stmSettings );
    
    stmSettings.uMemorySize =  2 * deviceSettings.uGranularity * 300;
    
    
    // Customize the Stream Manager settings here.

    if ( !AK::StreamMgr::Create( stmSettings ) )
    {
        assert( ! "Could not create the Streaming Manager" );
    }

   
    // Customize the streaming device settings here.
    m_lowLevelIO = new CAkDefaultIOHookBlocking();

    // CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
    // in the Stream Manager, and registers itself as the File Location Resolver.
    if ( m_lowLevelIO->Init( deviceSettings ) != AK_Success )
    {
        assert( ! "Could not create the streaming device and Low-Level I/O system" );
    }

    AkInitSettings initSettings;
    AkPlatformInitSettings platformInitSettings;
    AK::SoundEngine::GetDefaultInitSettings( initSettings );
    AK::SoundEngine::GetDefaultPlatformInitSettings( platformInitSettings );

    initSettings.uDefaultPoolSize           = 4 * 1024 * 1024;  // 4 MB
    //initSettings.uMaxNumPaths               = 40;
    //initSettings.uMaxNumTransitions         = 40;

    platformInitSettings.uLEngineDefaultPoolSize    = 10 * 1024 * 1024;  // 4 MB

    if ( AK::SoundEngine::Init( &initSettings, &platformInitSettings ) != AK_Success )
    {
        assert( ! "Could not initialize the Sound Engine." );
    }

    //
    // Initialize the music engine
    // Using default initialization parameters
    //

    AkMusicSettings musicInit;
    AK::MusicEngine::GetDefaultInitSettings( musicInit );

    if ( AK::MusicEngine::Init( &musicInit ) != AK_Success )
    {
        assert( ! "Could not initialize the Music Engine." );
    }

    AK::SoundEngine::RegisterAllPlugins();

    #ifdef _DEBUG

    #ifndef AK_OPTIMIZED
    //
    // Initialize communications (not in release build!)
    //
    AkCommSettings commSettings;
    AK::Comm::GetDefaultInitSettings( commSettings );

    if ( AK::Comm::Init( commSettings ) != AK_Success )
    {
        assert( ! "Could not initialize communication." );
        //return false;
    }

    #endif // AK_OPTIMIZED
    #endif



    Load( mConfigPath );
}

void CWWSoundManager::Update()
{
  BROFILER_CATEGORY( "CWWSoundManager::Update()", Profiler::Color::Orchid )
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
            const std::string& TagName = sounds_TreeNode( i ).GetName();

            if ( TagName == "Bank" )
            {
                const std::string& StrBankName = std::string( sounds_TreeNode( i ).GetAttribute<std::string>( "name", "" ) );

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

                const std::string& StrGameObject2dName = std::string( sounds_TreeNode( i ).GetAttribute<std::string>( "name", "" ) );
                const std::string& StrRegisteredGameObject2d = std::string( sounds_TreeNode( i ).GetAttribute<std::string>( "register", "" ) );

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

                const std::string& StrGameObject3dName = std::string( sounds_TreeNode( i ).GetAttribute<std::string>( "name", "" ) );
                const std::string& StrRegisteredGameObject3d = std::string( sounds_TreeNode( i ).GetAttribute<std::string>( "register", "" ) );

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
                        l_PosList[k] = Math::Vect3f( sounds_TreeNode( i )( k ).GetAttribute<Math::Vect3f>( "pos", Math::v3fZERO ) );
                        l_OrientationList[k] = Math::Vect3f( sounds_TreeNode( i )( k ).GetAttribute<Math::Vect3f>( "dir", Math::v3fZERO ) );
                    }

                    const std::string& l_TypeSource = sounds_TreeNode( i ).GetAttribute<std::string>( "type", "no_type" );

                    SetGameObjectMultiplePositions( StrGameObject3dName.c_str(), l_PosList, l_OrientationList, l_TypeSource );

                }
                else
                {
                    Math::Vect3f PosGameObject3d = Math::Vect3f( sounds_TreeNode( i ).GetAttribute<Math::Vect3f>( "pos", Math::Vect3f( 0.0f,
                                                   0.0f, 0.0f ).GetNormalized() ) );
                    Math::Vect3f DirGameObject3d = Math::Vect3f( sounds_TreeNode( i ).GetAttribute<Math::Vect3f>( "dir", Math::Vect3f( 0.0f,
                                                   0.0f, 0.0f ).GetNormalized() ) );

                    SetGameObjectPosition( StrGameObject3dName.c_str(), PosGameObject3d, DirGameObject3d );
                }
            }

            if ( TagName == "InitEvent" )
            {

                const std::string& l_name = std::string( sounds_TreeNode( i ).GetAttribute<std::string>( "name", "" ) );
                const std::string& l_gameObject = std::string( sounds_TreeNode( i ).GetAttribute<std::string>( "gameObject", "" ) );

                PlayEvent( l_name, l_gameObject );

            }

        }
    }

    return true;
}

void CWWSoundManager::LoadParticlePosSound(const std::string& aFileName)
{
  //std::map<std::string,std::vector<Math::Vect3f>> lTempInstanceParticleMap;
  CXMLTreeNode l_XML, l_Node;
  if ( l_XML.LoadAndFindNode( aFileName.c_str(), "particles_instances", l_Node ) )
  {
    for ( uint32 i = 0, lCount = l_Node.GetNumChildren(); i < lCount; ++i )
    {
      
      const CXMLTreeNode& lCurrentParticle = l_Node( i );
      const std::string& lCoreSoundParticle = lCurrentParticle.GetAttribute<std::string>( "core", "" );
      const std::string& lInstanceNameSoundParticle = lCurrentParticle.GetAttribute<std::string>( "name", "" );
      const std::string& lHasSound = lCurrentParticle.GetAttribute<std::string>( "sound", "" );
      const std::string& lIsActive = lCurrentParticle.GetAttribute<std::string>( "active", "" );
      Math::Vect3f lParticleSoundPos = lCurrentParticle.GetAttribute<Math::Vect3f>( "pos", 0 );
      

      if (lHasSound != "true" || lIsActive != "true")
        continue;
    
      m_ParticlesSoundMap[lInstanceNameSoundParticle] = lCoreSoundParticle;
      
      m_GameObjectMap[lInstanceNameSoundParticle.c_str()] =  ++m_ObjectId;

      AKRESULT l_AkResult =  RegisterGameObject( lInstanceNameSoundParticle.c_str() );

      if ( l_AkResult != AK_Success )
      {
        LOG_ERROR_APPLICATION( "CWWSoundManager::LoadCoreSoundParticles --> Error loading particle emitter %s.",
          lCoreSoundParticle.c_str() );
        return;
      }else{
        SetGameObjectPosition(lInstanceNameSoundParticle,lParticleSoundPos,lParticleSoundPos);
      }
    }

    /*typedef std::map<std::string, std::vector<Math::Vect3f>>::iterator it_type;
    for(it_type iterator = m_ParticlesSoundMap.begin(); iterator != m_ParticlesSoundMap.end(); iterator++) {
      std::vector<Math::Vect3f> lVectorOrientation((iterator->second).size(), 0);
      //SetGameObjectMultiplePositions( iterator->first, iterator->second, lVectorOrientation, "MultiSource" );
      
    }*/
  }

}

AKRESULT CWWSoundManager::SetSwitch( const std::string& _Group, const std::string& _Gravel, const std::string& _KeyGameObjectMap )
{
    return AK::SoundEngine::SetSwitch( _Group.c_str(), _Gravel.c_str(), m_GameObjectMap[_KeyGameObjectMap] );
}

AKRESULT CWWSoundManager::RegisterGameObject( const std::string& _KeyGameObjectMap )
{
    return AK::SoundEngine::RegisterGameObj( m_GameObjectMap[_KeyGameObjectMap], _KeyGameObjectMap.c_str() );
}

AKRESULT CWWSoundManager::UnregisterGameObject( const std::string& _KeyGameObjectMap )
{
    return AK::SoundEngine::UnregisterGameObj( m_GameObjectMap[_KeyGameObjectMap] );
}

AKRESULT CWWSoundManager::SetListenerPosition( Math::Vect3f _ListenerPosition, Math::Vect3f _Direction,
        Math::Vect3f _Up )
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

AKRESULT CWWSoundManager::SetGameObjectPosition( const std::string &_KeyGameObjectMap, Math::Vect3f _GameObjectPosition,
        Math::Vect3f _GameObjectOrientation )
{
    bool l_OK = true;
    AkSoundPosition l_AKGameObjectPosition;

    //_GameObjectOrientation = _GameObjectOrientation.Normalize();

    if ( _isnan( _GameObjectOrientation.x ) || _isnan( _GameObjectOrientation.y ) || _isnan( _GameObjectOrientation.z ) )
    {
        l_OK = false;
    }

    l_AKGameObjectPosition.Position.X = _GameObjectPosition.x;
    l_AKGameObjectPosition.Position.Y = _GameObjectPosition.y;
    l_AKGameObjectPosition.Position.Z = _GameObjectPosition.z;

    l_AKGameObjectPosition.Orientation.X = ( !l_OK ) ? 1.0f : _GameObjectOrientation.Normalize().x;
    l_AKGameObjectPosition.Orientation.Y = ( !l_OK ) ? 0.0f : _GameObjectOrientation.Normalize().y;
    l_AKGameObjectPosition.Orientation.Z = ( !l_OK ) ? 0.0f : _GameObjectOrientation.Normalize().z;

    return AK::SoundEngine::SetPosition( m_GameObjectMap[_KeyGameObjectMap] , l_AKGameObjectPosition );
}

AKRESULT CWWSoundManager::SetGameObjectMultiplePositions( const std::string& _KeyGameObjectMap,
        std::vector<Math::Vect3f> _GameObjectPosition,
        std::vector<Math::Vect3f> _GameObjectOrientation, const std::string& _TypePos )
{

    int l_NumPositions = _GameObjectPosition.size();

    AK::SoundEngine::MultiPositionType l_eMultiPositionType = AK::SoundEngine::MultiPositionType_MultiSources;

    AkSoundPosition* l_AKGameObjectPositions = ( AkSoundPosition* ) malloc( sizeof( AkSoundPosition ) * l_NumPositions );
    ZeroMemory( l_AKGameObjectPositions, sizeof( AkSoundPosition ) * l_NumPositions );

    for ( int i = 0; i < l_NumPositions; ++i )
    {
        l_AKGameObjectPositions[i].Position.X = _GameObjectPosition[i].x;
        l_AKGameObjectPositions[i].Position.Y = _GameObjectPosition[i].y;
        l_AKGameObjectPositions[i].Position.Z = _GameObjectPosition[i].z;

       /* l_AKGameObjectPositions[i].Orientation.X = _GameObjectOrientation[i].x;
        l_AKGameObjectPositions[i].Orientation.Y = _GameObjectOrientation[i].y;
        l_AKGameObjectPositions[i].Orientation.Z = _GameObjectOrientation[i].z;*/
    }

    if ( _TypePos == "MultiSource" )
    {
        l_eMultiPositionType = AK::SoundEngine::MultiPositionType_MultiSources;
    }
    else
    {
        l_eMultiPositionType = AK::SoundEngine::MultiPositionType_MultiDirections;
    }

    AKRESULT result = AK::SoundEngine::SetMultiplePositions( m_GameObjectMap[_KeyGameObjectMap] , l_AKGameObjectPositions,l_NumPositions, l_eMultiPositionType );
    
    if ( result != AK_Success )
    {
      LOG_ERROR_APPLICATION( "CWWSoundManager::SetMultiplePositions --> Error loading multiple position");
      CHECKED_DELETE( l_AKGameObjectPositions );
      return result;
    }
    return result;
}

void CWWSoundManager::PlayParticlesSFX()
{
  typedef std::map<std::string, std::string>::iterator it_type;
  
  for(it_type iterator = m_ParticlesSoundMap.begin(); iterator != m_ParticlesSoundMap.end(); iterator++) 
  {
    PlayEvent("Play_"+iterator->second,iterator->first);
  }

}

AKRESULT CWWSoundManager::SetState( const std::string& _Group, const std::string& _State )
{
    return  AK::SoundEngine::SetState( _Group.c_str(), _State.c_str() );
}

AkPlayingID CWWSoundManager::PlayEvent( const std::string& _Event,  const std::string& _KeyGameObjectMap )
{
    return AK::SoundEngine::PostEvent( _Event.c_str(), m_GameObjectMap[_KeyGameObjectMap] );
}

void CWWSoundManager::PauseEvent( const std::string& _Event,  const std::string& _KeyGameObjectMap )
{
    if ( AK::SoundEngine::IsInitialized() )
    {
        std::string lMsg = "Playing " + _Event + " event in " + _KeyGameObjectMap + " game object.";
        LOG_INFO_APPLICATION(lMsg.c_str());
        AK::SoundEngine::PostEvent( _Event.c_str(), m_GameObjectMap[_KeyGameObjectMap] );
        AK::SoundEngine::RenderAudio();

        AK::SoundEngine::Suspend( false );
        SetGamePaused( true );
    }
}

void CWWSoundManager::ResumeEvent( const std::string& _Event,  const std::string& _KeyGameObjectMap )
{
    if ( AK::SoundEngine::IsInitialized() )
    {
        AK::SoundEngine::WakeupFromSuspend();

        AK::SoundEngine::PostEvent( _Event.c_str(), m_GameObjectMap[_KeyGameObjectMap] );
        AK::SoundEngine::RenderAudio();

        SetGamePaused( false );
    }
}


void CWWSoundManager::PauseAllFromGameObject( const std::string& _KeyGameObjectMap )
{
    if ( AK::SoundEngine::IsInitialized() )
    {
        AK::SoundEngine::PostEvent( "PauseAll", m_GameObjectMap[_KeyGameObjectMap] );
        AK::SoundEngine::RenderAudio();

        AK::SoundEngine::Suspend( false );
        SetGamePaused( true );
    }
}

void CWWSoundManager::ResumeAllFromGameObject( const std::string& _KeyGameObjectMap )
{
    if ( AK::SoundEngine::IsInitialized() )
    {
        AK::SoundEngine::WakeupFromSuspend();

        AK::SoundEngine::PostEvent( "ResumeAll", m_GameObjectMap[_KeyGameObjectMap] );
        AK::SoundEngine::RenderAudio();

        SetGamePaused( false );
    }
}

AkGameObjectID CWWSoundManager::GetGameObjectMapById( const std::string& _KeyGameObjectMap )
{
    return m_GameObjectMap[_KeyGameObjectMap];
}

void CWWSoundManager::SetGameObjectMapById( const std::string& _KeyGameObjectMap )
{
    m_GameObjectMap[_KeyGameObjectMap] =  ++m_ObjectId;
}


