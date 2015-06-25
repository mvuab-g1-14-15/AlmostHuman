#include "AnimatedModelsManager.h"
#include "AnimatedCoreModel.h"
#include "AnimatedInstanceModel.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "EngineConfig.h"

CAnimatedModelsManager::CAnimatedModelsManager()
    : CManager()
{
    CalLoader::setLoadingMode( LOADER_ROTATE_X_AXIS );
}

CAnimatedModelsManager::CAnimatedModelsManager( CXMLTreeNode& atts )
    : CManager( atts )
{
    CalLoader::setLoadingMode( LOADER_ROTATE_X_AXIS );
}

CAnimatedModelsManager::~CAnimatedModelsManager()
{
    Destroy();
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
    CAnimatedCoreModel* l_pAnimatedCoreModel = new CAnimatedCoreModel( Name );

    if ( !l_pAnimatedCoreModel->Load( Path ) )
    {
        CHECKED_DELETE( l_pAnimatedCoreModel );
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