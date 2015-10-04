#include "BillboardInstanceManager.h"
#include "BillboardInstance.h"
#include "GraphicsManager.h"

CBillboardInstanceManager::CBillboardInstanceManager( const CXMLTreeNode& atts )
  : CManager( atts )
{
}

CBillboardInstanceManager::~CBillboardInstanceManager()
{
}

void CBillboardInstanceManager::Render()
{
}

void CBillboardInstanceManager::Update()
{
}

void CBillboardInstanceManager::Init()
{
  CXMLTreeNode newFile, node;

  if ( newFile.LoadAndFindNode( mConfigPath.c_str(), "billboard_instances", node ) )
  {
    for ( uint32 i = 0, lCount = node.GetNumChildren(); i < lCount ; ++i )
    {
      const CXMLTreeNode& lCurrentNode = node( i );
      const std::string& lCurrentNodeName = lCurrentNode.GetName();
      if( lCurrentNodeName == "billboard_instance" )
      {
        const std::string& lBillboardName = lCurrentNode.GetAttribute<std::string>( "name", "unknown" );
        const std::string& lBillboardCore = lCurrentNode.GetAttribute<std::string>( "core", "unknonw" );

        CBillboardInstance* lBillBoard = new CBillboardInstance();
        if ( !lBillBoard->Init( lCurrentNode ) || !AddResource( lBillboardName, lBillBoard ))
        {
          CHECKED_DELETE( lBillBoard );
          LOG_ERROR_APPLICATION( "Error creating the billboard %s check the configuration file!", lBillboardName.c_str() );
        }
    }
  }
}