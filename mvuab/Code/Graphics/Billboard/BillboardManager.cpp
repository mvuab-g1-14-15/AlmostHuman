#include "BillboardManager.h"
#include "GraphicsManager.h"
#include "Billboard.h"

#include "Memory\FreeListAllocator.h"
#include "Memory\AllocatorManager.h"
#include "Memory\LinearAllocator.h"

#include "EngineManagers.h"

CBillboardManager::CBillboardManager( const CXMLTreeNode& atts ) : CManager( atts )
{
}

CBillboardManager::~CBillboardManager()
{
    CBillboard::DestroyBillBoardGeometry();
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    //Destroy();//TODO ALEX FALTABA DESTRUIR LOS BILLBOARDS

    for ( TMapResource::iterator lItb = m_Resources.begin(), lIte = m_Resources.end(); lItb != lIte; ++lItb )
    {
        l_AllocatorManger->m_pFreeListAllocator->MakeDelete(lItb->second);
    }
    
    m_Resources.clear();
}

void CBillboardManager::Render()
{
    for ( TMapResource::iterator lItb = m_Resources.begin(), lIte = m_Resources.end(); lItb != lIte; ++lItb )
    {
        lItb->second->Render();
    }
}

void CBillboardManager::Update()
{
}

void CBillboardManager::Init()
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

    // Create the shared geometry
    CBillboard::CreateBillBoardGeometry();
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    for ( uint32 i = 0, lCount = node.GetNumChildren(); i < lCount ; ++i )
    {
        const CXMLTreeNode& lCurrentNode = node(i);
        const std::string& lCurrentNodeName = lCurrentNode.GetName();

        if ( lCurrentNodeName == "billboard" )
        {
            const std::string& lBillboardName = lCurrentNode.GetAttribute<std::string>( "name", "unknown" );
            CBillboard* lBillBoard = l_AllocatorManger->m_pFreeListAllocator->MakeNew<CBillboard>();

            if ( lBillBoard->Init( lCurrentNode ) )
            {
                AddResource( lBillboardName, lBillBoard );
            }
            else
            {
                l_AllocatorManger->m_pFreeListAllocator->MakeDelete( lBillBoard );
                LOG_ERROR_APPLICATION( "Error creating the billboard %s check the configuration file!", lBillboardName.c_str() );
            }
        }
    }
}