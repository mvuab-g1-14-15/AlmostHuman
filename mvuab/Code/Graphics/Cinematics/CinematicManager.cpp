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

ObjectFactory1<CCinematicsElement, CXMLTreeNode, std::string > CommandFactory;

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
  CommandFactory.Register( "wait_time", Type2Type<CWaitTimeElement>( ) );
  CommandFactory.Register( "show_dialog", Type2Type<CShowDialogElement>( ) );
  CommandFactory.Register( "hide", Type2Type<CHideElement>( ) );

  // Register all the elements with the object factory class
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
      CXMLTreeNode  SubTreeNode = TreeNode( i );
      const std::string& TagName = SubTreeNode.GetName();

      if ( TagName == "comment" )
        continue;

      std::string ResourceFileName = SubTreeNode.GetPszProperty( "resource_id_file", GetNextName().c_str());

	    CCinematicsItems* CinematicsItems = new CCinematicsItems( ResourceFileName );


      if ( !CinematicsItems )
        LOG_ERROR_APPLICATION( "Comand %s not found in the factory of commands!", TagName.c_str() );
      else
      {
        if ( !m_vCinematicsElement.AddResource( SubTreeNode.GetPszProperty( "name",
             GetNextName().c_str() ), CinematicsItems ) )
          CHECKED_DELETE( CinematicsItems );
      }
    }
  }
}

void CCinematicManager::Execute(const std::string& NameCinematic)
{
  m_vCinematicsElement.GetResource(NameCinematic)->Execute();
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
        LOG_ERROR_APPLICATION( "Comand %s not found in the factory of commands!", TagName.c_str() );
      else
        m_CinematicsItems.push_back( CinematicsElement );
    }
  }
}

void CCinematicManager::CCinematicsItems::Execute()
{
  CGraphicsManager* gm = GraphicsInstance;
  std::vector<CCinematicsElement*>::iterator it = m_CinematicsItems.begin(),
                                             it_end = m_CinematicsItems.end();
  for ( ; it != it_end; ++it )
    ( *it )->Execute( *gm );
}