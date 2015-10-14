#include "Cinematics\CinematicManager.h"
#include "Cinematics\CinematicsElement\ShowDialogElement.h"
#include "Cinematics\CinematicsElement\HideElement.h"
#include "Cinematics\CinematicsElement\PlayAnimationElement.h"
#include "Cinematics\CinematicsElement\SetCameraElement.h"
#include "Cinematics\CinematicsElement\SetTransformElement.h"
#include "Cinematics\CinematicsElement\WaitTimeElement.h"
#include "Cameras\CameraCinematical.h"
#include "Cameras\CameraManager.h"
#include "Cinematics\Cinematic.h"
#include "XML\XMLTreeNode.h"
#include "EngineConfig.h"
#include "Timer\Timer.h"
#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Utils\TemplatedVectorMapManager.h"

#include <Windows.h>
#include <sstream>
#include "Utils\ObjectFactory.h"

ObjectFactory1<CCinematicsElement, CXMLTreeNode, std::string > CommandFactory;

CCinematicManager::CCinematicManager()
    : CManager()
{
}

CCinematicManager::CCinematicManager( CXMLTreeNode& atts )
    : CManager( atts )
    , m_CurrentCinematicsElement( 0 )
    , m_CurrentElement( 0 )
    , m_CurrentElementId( 0 )
    , m_CheckBlock( true )
    , m_CinematicActive( false )
    , m_FirstFrame( true )
{
    m_CurrentCamera = "";
}

CCinematicManager::~CCinematicManager()
{
    m_vCinematicsElement.Destroy();
}

void CCinematicManager::CleanUp()
{
    m_vCinematicsElement.Destroy();
}
std::string CCinematicManager::GetNextName()
{
    std::stringstream l_Str;
    l_Str << "default_cinematics_element_";
    l_Str << m_vCinematicsElement.GetResourcesVector().size();
    return l_Str.str();
}

void CCinematicManager::Init()
{
    CommandFactory.Register( "set_transform", Type2Type<CSetTransformElement>( ) );
    CommandFactory.Register( "play_animation", Type2Type<CPlayAnimationElement>( ) );
    CommandFactory.Register( "set_camera", Type2Type<CSetCameraElement>( ) );
    CommandFactory.Register( "wait", Type2Type<CWaitTimeElement>( ) );
    CommandFactory.Register( "show_dialog", Type2Type<CShowDialogElement>( ) );
    CommandFactory.Register( "hide", Type2Type<CHideElement>( ) );
}

void CCinematicManager::LoadCinematics( const std::string& aCinematicsFile )
{
    // Register all the elements with the object factory class
    CXMLTreeNode l_File, TreeNode;
    if ( l_File.LoadAndFindNode( aCinematicsFile.c_str(), "cinematics", TreeNode ) )
    {
        for ( int i = 0, count = TreeNode.GetNumChildren(); i < count; ++i )
        {
            CXMLTreeNode  SubTreeNode = TreeNode( i );
            const std::string& TagName = SubTreeNode.GetName();
            const std::string& ResourceFileName = SubTreeNode.GetAttribute<std::string>( "resource_id_file", GetNextName().c_str() );

            if ( TagName == "cinematic" )
            {
                CCinematicsItems* CinematicsItems = new CCinematicsItems( ResourceFileName );

                if ( !CinematicsItems )
                {
                    LOG_ERROR_APPLICATION( "Error making %s!", TagName.c_str() );
                }
                else
                {
                    if ( !m_vCinematicsElement.AddResource( SubTreeNode.GetAttribute<std::string>( "name",
                        GetNextName().c_str() ), CinematicsItems ) )
                    {
                        CHECKED_DELETE( CinematicsItems );
                    }
                }
            }
            else if( TagName == "camera_cinematic" )
            {
                const std::string& lName = SubTreeNode.GetAttribute<std::string>( "name", "" );
                CXMLTreeNode l_SubFile, SubTreeNode3;
                if (l_SubFile.LoadAndFindNode( ResourceFileName.c_str(), "camera", SubTreeNode3 ) )
                {
                    CCameraKeyController* CameraKeyController = new CCameraKeyController( );
                    CameraKeyController->LoadXML(SubTreeNode3(0));
                    CCameraCinematical* lCameraCinematical = new CCameraCinematical( CameraKeyController );
                    if ( !CameraMInstance->AddResource( lName, lCameraCinematical ) )
                    {
                        LOG_ERROR_APPLICATION( "Error making %s!", TagName.c_str() );
                        CHECKED_DELETE( lCameraCinematical );
                        CHECKED_DELETE( CameraKeyController );
                    }
                }
            }
            else if( TagName == "object_cinematic" )
            {
                const std::string& lName = SubTreeNode.GetAttribute<std::string>( "name", "" );
                CCinematic* ObjectCinematic = new CCinematic(ResourceFileName);
                ObjectCinematic->Play(true);
                if ( !m_vCinematicLoop.AddResource( lName, ObjectCinematic ) )
                {
                    LOG_ERROR_APPLICATION( "Error making %s!", TagName.c_str() );
                    CHECKED_DELETE( ObjectCinematic );
                }
            }
        }
    }
}

void CCinematicManager::Execute( const std::string& NameCinematic )
{
    m_CurrentCinematicsElement = m_vCinematicsElement.GetResource( NameCinematic );
    m_CurrentElement = m_CurrentCinematicsElement->m_CinematicsItems.GetResourceById( m_CurrentElementId );
}

void CCinematicManager::ReLoad()
{
    CleanUp();
    Init();
}

CCinematicManager::CCinematicsItems::CCinematicsItems( const std::string& Node )
{
    CXMLTreeNode lFile;

    if ( !lFile.LoadFile( Node.c_str() ) )
    {
        FATAL_ERROR( "Error loading the file %s", Node.c_str() );
    }

    if ( lFile.Exists() )
    {
        for ( int i = 0, count = lFile.GetNumChildren(); i < count; ++i )
        {
            const CXMLTreeNode& TreeNode = lFile( i );
            const std::string& TagName = TreeNode.GetName();

            CCinematicsElement* CinematicsElement = 0;

            CinematicsElement = CommandFactory.Create( TagName.c_str(), TreeNode );

            if ( !CinematicsElement )
            {
                LOG_ERROR_APPLICATION( "CinematicElement %s not found in the factory of CinematicsElement!", TagName.c_str() );
            }
            else if ( !m_CinematicsItems.AddResource( TreeNode.GetAttribute<std::string>( "name",
                      GetNextName().c_str() ), CinematicsElement ) )
            {
                CHECKED_DELETE( CinematicsElement );
            }
        }
    }
}

void CCinematicManager::CCinematicsItems::Execute()
{
    CGraphicsManager* gm = GraphicsInstance;
    std::vector<CCinematicsElement*>::iterator it = m_CinematicsItems.GetResourcesVector().begin(),
                                               it_end = m_CinematicsItems.GetResourcesVector().end();

    for ( ; it != it_end; ++it )
    {
        ( *it )->Execute( *gm );
    }
}


std::string CCinematicManager::CCinematicsItems::GetNextName()
{
    std::stringstream l_Str;
    l_Str << "default_cinematics_item_";
    l_Str << m_CinematicsItems.GetResourcesVector().size();
    return l_Str.str();
}


void CCinematicManager::Update()
{
    if( m_vCinematicLoop.GetResourcesCount() != 0 )
	{
		std::vector<CCinematic*>::iterator it = m_vCinematicLoop.GetResourcesVector().begin(),
												   it_end = m_vCinematicLoop.GetResourcesVector().end();
		for(; it != it_end; ++it)
		{
			(*it)->Update();
		}
	}

    if ( m_CurrentElement != 0 )
    {
        //Compruebo si este elemento es bloqueante y no está bloqueado por otro
        if ( m_CheckBlock && !m_CinematicActive )
        {
            m_CinematicActive = m_CurrentElement->GetIsBlocker();
        }

        m_CheckBlock = false;

        //Compruebo si ha transcurrido el tiempo para esta acción para cambiar al siguiente elemento
        if ( m_CurrentElement->GetTime() < m_CurrentElement->GetCurrentTime() )
        {
            m_CurrentElement->SetCurrentTime( 0 );
            m_CurrentElement = m_CurrentCinematicsElement->m_CinematicsItems.GetResourceById( ++m_CurrentElementId );
            m_CheckBlock = true;
            m_FirstFrame = true;

            if ( m_CurrentElement == 0 )
            {
                m_CurrentElementId = 0;
                m_CinematicActive = false;
                return;
            }
        }

        if ( !m_FirstFrame )
        {
            m_CurrentElement->Update();
        }

        m_FirstFrame = false;
    }
}

void CCinematicManager::Render()
{
    if ( m_CurrentElement != 0 )
    {
        m_CurrentElement->Render();
    }
}

void CCinematicManager::PlayCinematic( const std::string& aName )
{
    m_CurrentCamera = CameraMInstance->GetCurrentCameraName();
    CameraMInstance->SetCurrentCamera( aName );
    CameraMInstance->GetCurrentCamera()->SetEnable(true);
    m_CinematicActive = true;
}

void CCinematicManager::StopCinematic( const std::string& aName )
{
    CameraMInstance->GetCurrentCamera()->SetEnable(false);
    CameraMInstance->SetCurrentCamera( m_CurrentCamera );
    m_CinematicActive = false;
}