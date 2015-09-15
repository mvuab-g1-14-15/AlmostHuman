#include "BillboardManager.h"
#include "Billboard.h"
#include "GraphicsManager.h"

CBillboardManager::CBillboardManager( const CXMLTreeNode& atts )
  : CManager( atts )
{
}

CBillboardManager::~CBillboardManager()
{
  CBillboard::DestroyBillBoardGeometry();
  Destroy();//TODO ALEX FALTABA DESTRUIR LOS BILLBOARDS
}

void CBillboardManager::Render()
{
  for ( TMapResource::iterator lItb = m_Resources.begin(), lIte = m_Resources.end(); lItb != lIte; ++lItb )
    lItb->second->Render();
}

void CBillboardManager::Update()
{
}

void CBillboardManager::Init()
{
  // Create the shared geometry
  CBillboard::CreateBillBoardGeometry();
  LoadFromXML();
}

void CBillboardManager::Reload()
{
  Destroy();
  LoadFromXML();
}

void CBillboardManager::LoadFromXML()
{
  CXMLTreeNode newFile;

  if ( !newFile.LoadFile( mConfigPath.c_str() ) )
  {
    LOG_ERROR_APPLICATION( "Unable to open the file %s", mConfigPath.c_str() );
    return;
  }

  CXMLTreeNode node = newFile["billboards"];

  if ( !node.Exists() )
  {
    LOG_ERROR_APPLICATION( "Tag %s not found",  "billboards" );
    return;
  }

  for ( uint32 i = 0, lCount = node.GetNumChildren(); i < lCount ; ++i )
  {
    const CXMLTreeNode& lCurrentNode = node( i );
    const std::string& lCurrentNodeName = lCurrentNode.GetName();

    if ( lCurrentNodeName == "billboard" )
    {
      const std::string& lBillboardName = lCurrentNode.GetAttribute<std::string>( "name", "unknown" );
      CBillboard* lBillBoard = new CBillboard();

      if ( lBillBoard->Init( lCurrentNode ) )
        AddResource( lBillboardName, lBillBoard );
      else
      {
        CHECKED_DELETE( lBillBoard );
        LOG_ERROR_APPLICATION( "Error creating the billboard %s check the configuration file!", lBillboardName.c_str() );
      }
    }
  }
}