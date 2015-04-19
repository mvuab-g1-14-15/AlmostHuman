#include "Core.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "Language\LanguageManager.h"
#include "Fonts\FontManager.h"
#include "ActionManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "AnimatedModels\AnimatedModelsManager.h"
#include "Texture\TextureManager.h"
#include "RenderableObject\RenderableObjectsManager.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "ScriptManager.h"
#include "Cameras\CameraManager.h"
#include "Effects\EffectManager.h"
#include "Lights\LightManager.h"
#include "Timer\Timer.h"
#include "Console\Console.h"
#include "PhysicsManager.h"
#include "Triggers\TriggerManager.h"
#include "Actor\PhysicActor.h"
#include "Characters\Enemies\EnemyManager.h"
#include "Billboard\Billboard.h"
#include "Particles\ParticleManager.h"
#include "Gizmos\GizmosManager.h"
#include "EngineConfig.h"
#include "EngineManagers.h"

#include "SceneRenderComands\SceneRendererCommandManager.h"

#include <iostream>
#include "SoundManager.h"

CCore::CCore()
    : m_pTimer( 0 )
    , m_pConsole( 0 )
      // , m_pEngineManagers( 0 )
{
}

CCore::~CCore()
{
    // Destroy();
    CHECKED_DELETE( m_pTimer );
    CHECKED_DELETE( m_pConsole );
}

void CCore::Init()
{
    // Obtain the path of the configuration file and parse it
    CreateManagers();
    InitManagers();
}

void CCore::Update()
{
    //m_pEngineManagers->Update();
    m_pTimer->Update();
    if ( ActionManagerInstance->DoAction( "ClearConsole" ) )
    {
        if ( m_pConsole )
        { m_pConsole->Clear(); }
    }
}

void CCore::Render()
{
    SRCMInstance->Execute();
}
void CCore::CreateManagers()
{
    if ( EngineConfigInstance->GetEnableConsole() )
    { CreateConsole(); }
    m_pTimer = new CTimer( 30 );
}

void CCore::CreateConsole()
{
    m_pConsole = new CConsole( TRUE );
    m_pConsole->RedirectToConsole( 0 );
    m_pConsole->SetNumberOfLines( 800 );
    m_pConsole->SetNumberOfColumns( 132 );
    m_pConsole->SetMaxLinesInWindow( 25 );
    WORD x = m_pConsole->SetAttributes( FOREGROUND_BLUE, 2 );
    m_pConsole->SetAttributes( x );
    m_pConsole->SetFullSize();
}

void CCore::InitManagers()
{
}

void CCore::Trace( const std::string& TraceStr )
{
    HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
    SetConsoleTextAttribute( hConsole,
                             FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
    std::cout << TraceStr << std::endl << std::endl;
}