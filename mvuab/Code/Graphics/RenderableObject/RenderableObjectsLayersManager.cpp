#include "RenderableObjectsLayersManager.h"
#include "StaticMeshes\InstanceMesh.h"

CRenderableObjectsLayersManager::CRenderableObjectsLayersManager()
{

}

CRenderableObjectsLayersManager::~CRenderableObjectsLayersManager()
{
  Destroy();
}
void CRenderableObjectsLayersManager::Destroy()
{
  CTemplatedVectorMapManager::Destroy();
}
void CRenderableObjectsLayersManager::Load( const std::string& FileName )
{
  m_FileName = FileName;
  CXMLTreeNode l_File;

  if ( !l_File.LoadFile( m_FileName.c_str() ) )
  {
    std::string err = "ERROR reading the file " + FileName;

    MessageBox( NULL, err.c_str() , "Error", MB_ICONEXCLAMATION | MB_OK );
    exit( EXIT_FAILURE );
  }

  CXMLTreeNode  TreeNode = l_File["RenderableObjects"];

  if ( TreeNode.Exists() )
  {
    int count = TreeNode.GetNumChildren();

    for ( int i = 0; i < count; ++i )
    {
      const std::string& TagName = TreeNode( i ).GetName();

      if ( TagName == "layer" && TreeNode( i ).GetBoolProperty( "default", false ) )
      {
        m_DefaultRenderableObjectManager = new CRenderableObjectsManager();

        if ( !AddResource( TreeNode( i ).GetPszProperty( "name", "" ), m_DefaultRenderableObjectManager ) )
          CHECKED_DELETE( m_DefaultRenderableObjectManager );
      }
      else if ( TagName == "layer" )
      {
        CRenderableObjectsManager* RenderableObjectManager = new CRenderableObjectsManager();

        if ( !AddResource( TreeNode( i ).GetPszProperty( "name", "" ), RenderableObjectManager ) )
          CHECKED_DELETE( RenderableObjectManager );
      }

      if ( TagName == "MeshInstance" )
      {
        CRenderableObjectsManager* RenderableObjectManager = GetRenderableObjectManager( TreeNode( i ) );
        const std::string& l_Name = TreeNode( i ).GetPszProperty( "name", "" );
        /*const bool      &l_Visible = TreeNode(i).GetBoolProperty("visible", false);
        const bool      &l_CreatePhysics = TreeNode(i).GetBoolProperty("create_physics", false);
        const std::string &l_Physics = TreeNode(i).GetPszProperty("physics_type", "");*/
        CInstanceMesh* l_InstanceMesh = new CInstanceMesh( TreeNode( i ) );

        if ( !RenderableObjectManager->AddResource( l_Name, l_InstanceMesh ) )
          CHECKED_DELETE( l_InstanceMesh );
      }
    }
  }
}
void CRenderableObjectsLayersManager::Reload()
{
  Destroy();
  Load( m_FileName );
}
void CRenderableObjectsLayersManager::Update()
{
  std::vector<CRenderableObjectsManager*>::iterator itb = GetResourcesVector().begin(), ite = GetResourcesVector().end();

  for ( ; itb != ite; ++itb )
    ( *itb )->Update();
}
void CRenderableObjectsLayersManager::Render()
{
  std::vector<CRenderableObjectsManager*>::iterator itb = GetResourcesVector().begin(), ite = GetResourcesVector().end();

  for ( ; itb != ite; ++itb )
    ( *itb )->Render();
}
void CRenderableObjectsLayersManager::Render( const std::string& LayerName )
{
  TMapResources l_ResourcesMap = GetResourcesMap();
  TMapResources::iterator itb = l_ResourcesMap.find( LayerName );

  if ( itb != l_ResourcesMap.end() )
    ( *itb ).second.m_Value->Render();
}
CRenderableObjectsManager* CRenderableObjectsLayersManager::GetRenderableObjectManager( CXMLTreeNode& Node )
{
  const std::string& l_Layer = Node.GetPszProperty( "layer", "" );
  return ( l_Layer == "" ) ? m_DefaultRenderableObjectManager : GetResource( l_Layer.c_str() );
}