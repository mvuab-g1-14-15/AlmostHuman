#include "AnimatedModelsManager.h"
#include "AnimatedCoreModel.h"
#include "AnimatedInstanceModel.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"

#include "EngineManagers.h"
#include "EngineConfig.h"

#include "Memory\FreeListAllocator.h"
#include "Memory\AllocatorManager.h"
#include "Memory\LinearAllocator.h"

CAnimatedModelsManager::CAnimatedModelsManager(): CManager()
{
    CalLoader::setLoadingMode( LOADER_ROTATE_X_AXIS );
}

CAnimatedModelsManager::CAnimatedModelsManager( CXMLTreeNode& atts ) : CManager( atts )
{
    CalLoader::setLoadingMode( LOADER_ROTATE_X_AXIS );
}

CAnimatedModelsManager::~CAnimatedModelsManager()
{
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    for(TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); it++)
    {
        l_AllocatorManger->m_pFreeListAllocator->MakeDelete(it->second);
        it->second = 0;
    }

    m_Resources.clear();
}

CAnimatedCoreModel* CAnimatedModelsManager::GetCore( const std::string& Name, const std::string& Path )
{
    CAnimatedCoreModel* l_pAnimatedCoreModel = GetResource( Name );

    // Check if the core model is already in memory
    if ( !l_pAnimatedCoreModel )
    {
        l_pAnimatedCoreModel = AddNewCore( Name, Path );
    }

    assert( l_pAnimatedCoreModel );

    return l_pAnimatedCoreModel;
}

CAnimatedCoreModel* CAnimatedModelsManager::GetCore( const std::string& Name )
{
    CAnimatedCoreModel* l_pAnimatedCoreModel = GetResource( Name );

    assert( l_pAnimatedCoreModel );

    return l_pAnimatedCoreModel;
}

CAnimatedInstanceModel* CAnimatedModelsManager::GetInstance( const std::string& Name )
{
    return 0;
}

void CAnimatedModelsManager::Init()
{
    CXMLTreeNode newFile;

    if ( !newFile.LoadFile( mConfigPath.c_str() ) )
    {
        LOG_ERROR_APPLICATION( "CAnimatedModelManager::Load No se puede abrir \"%s\"!", mConfigPath.c_str() );
        return;
    }

    CXMLTreeNode node = newFile["animated_models"];

    if ( !node.Exists() )
    {
        LOG_ERROR_APPLICATION( "CAnimatedModelManager::Load Tag \"%s\" no existe",  "animated_models" );
        return;
    }

    for ( uint32 i = 0, lCount = node.GetNumChildren(); i < lCount; ++i )
    {
        const CXMLTreeNode& lCurrentNode = node(i);
        const std::string& name = lCurrentNode.GetAttribute<std::string>( "name", "no_name" );
        const std::string& path = lCurrentNode.GetAttribute<std::string>( "path", "no_file" );
        AddNewCore( name, path );
    }
}

CAnimatedCoreModel* CAnimatedModelsManager::AddNewCore( const std::string& Name, const std::string& Path )
{
    CAllocatorManager *l_AllocatorManger = CEngineManagers::GetSingletonPtr()->GetAllocatorManager();

    CAnimatedCoreModel* l_pAnimatedCoreModel = (CAnimatedCoreModel *) l_AllocatorManger->m_pFreeListAllocator->Allocate(sizeof(CAnimatedCoreModel), __alignof(CAnimatedCoreModel));
    new (l_pAnimatedCoreModel) CAnimatedCoreModel(Name);

    if ( !l_pAnimatedCoreModel->Load( Path ) )
    {
        l_AllocatorManger->m_pFreeListAllocator->MakeDelete( l_pAnimatedCoreModel );
        l_pAnimatedCoreModel = 0;
    }
    else
    {
        AddResource( Name, l_pAnimatedCoreModel );
    }

    return l_pAnimatedCoreModel;
}

void CAnimatedModelsManager::Reload()
{
    TMapResource::iterator it = m_Resources.begin(),
                           it_end = m_Resources.end();

    for ( ; it != it_end; ++it )
    {
        it->second->Reload();
    }
}