#include "Cinematics\CinematicManager.h"
#include "Cinematics\CinematicsElement\ShowDialogElement.h"
#include "Cinematics\CinematicsElement\HideElement.h"
#include "Cinematics\CinematicsElement\PlayAnimationElement.h"
#include "Cinematics\CinematicsElement\SetCameraElement.h"
#include "Cinematics\CinematicsElement\SetTransformElement.h"
#include "Cinematics\CinematicsElement\WaitTimeElement.h"
#include "XML\XMLTreeNode.h"
#include "EngineConfig.h"

#include "GraphicsManager.h"
#include "EngineManagers.h"

#include <Windows.h>
#include <sstream>
#include "Utils\ObjectFactory.h"


CCinematicManager::CCinematicManager()
  : CManager()
{
}

CCinematicManager::CCinematicManager( CXMLTreeNode& atts )
  : CManager( atts )
{
  /*  TODO RAUL
      LEER XML
  */
}

CCinematicManager::~CCinematicManager()
{
  m_CinematicsElement.Destroy();
}

void CCinematicManager::CleanUp()
{
  m_CinematicsElement.Destroy();
}
std::string CCinematicManager::GetNextName()
{
  std::stringstream l_Str;
  l_Str << "default_cinematics_element_";
  l_Str << m_CinematicsElement.GetResourcesVector().size();
  return l_Str.str();
}

void CCinematicManager::Init()
{
  ObjectFactory1<CCinematicsElement, CXMLTreeNode, std::string > CommandFactory;
  // Register all the elements with the object factory class
  CommandFactory.Register( "set_transform", Type2Type<CSetTransformElement>( ) );
  CommandFactory.Register( "play_animation", Type2Type<CPlayAnimationElement>( ) );
  CommandFactory.Register( "set_camera", Type2Type<CSetCameraElement>( ) );
  CommandFactory.Register( "wait_time", Type2Type<CWaitTimeElement>( ) );
  CommandFactory.Register( "show_dialog", Type2Type<CShowDialogElement>( ) );
  CommandFactory.Register( "hide", Type2Type<CHideElement>( ) );
  CXMLTreeNode l_File;

  if ( !l_File.LoadFile( mConfigPath.c_str() ) )
  {
    const std::string& lMsgError = "Error reading the file " + mConfigPath;
    FATAL_ERROR( lMsgError.c_str() );
  }

  CXMLTreeNode  TreeNode = l_File["cinematics"];

  if ( TreeNode.Exists() )
  {
    int count = TreeNode.GetNumChildren();

    for ( int i = 0; i < count; ++i )
    {
      const std::string& TagName = TreeNode( i ).GetName();

      if ( TagName == "comment" )
        continue;

	  CCinematicsElement* CinematicsElement = 0;

      CinematicsElement = CommandFactory.Create( TagName.c_str(), TreeNode( i ) );

      if ( !CinematicsElement )
        LOG_ERROR_APPLICATION( "Comand %s not found in the factory of commands!", TagName.c_str() );
      else
      {
        if ( !m_CinematicsElement.AddResource( TreeNode( i ).GetPszProperty( "name",
             GetNextName().c_str() ), CinematicsElement ) )
          CHECKED_DELETE( CinematicsElement );
      }
    }
  }

  //  return true;
}

bool CCinematicManager::Execute()
{
  CGraphicsManager* gm = GraphicsInstance;
  std::vector<CCinematicsElement*>::iterator it =
    m_CinematicsElement.GetResourcesVector().begin(),
    it_end = m_CinematicsElement.GetResourcesVector().end();

  for ( ; it != it_end; ++it )
    ( *it )->Execute( *gm );

  return true;
}

void CCinematicManager::ReLoad()
{
  CleanUp();
  Init();
}
