#include "BillboardManager.h"
#include "Billboard.h"
#include "GraphicsManager.h"

CBillboardManager::CBillboardManager( const CXMLTreeNode& atts )
    : CManager(atts)
{
}

CBillboardManager::~CBillboardManager()
{
	CBillboard::DestroyBillBoardGeometry();
}

void CBillboardManager::Render()
{
}

void CBillboardManager::Update()
{
}

void CBillboardManager::Init()
{
	CXMLTreeNode newFile;
    if (!newFile.LoadFile(mConfigPath.c_str()))
    {
        LOG_ERROR_APPLICATION( "Unable to open the file %s", mConfigPath.c_str());
        return;
    }

    CXMLTreeNode node = newFile["billboards"];
    if(!node.Exists())
    {
        LOG_ERROR_APPLICATION( "Tag %s not found",  "billboards");
        return;
    }

	// Create the shared geometry
	CBillboard::CreateBillBoardGeometry();

    for(int i = 0; i < node.GetNumChildren(); i++)
    {
		const std::string& lCurrentNodeName = node(i).GetName();
		if( lCurrentNodeName == "billboard")
		{
			const std::string& lBillboardName = node(i).GetPszProperty("name", "unknown");
			CBillboard* lBillBoard = new CBillboard();
			if( lBillBoard->Init( node(i) ))
			{
				AddResource(lBillboardName, lBillBoard );
			}
			else
			{
				CHECKED_DELETE( lBillBoard );
				LOG_ERROR_APPLICATION("Error creating the billboard %s check the configuration file!", lBillboardName.c_str() );
			}
		}
	}
}