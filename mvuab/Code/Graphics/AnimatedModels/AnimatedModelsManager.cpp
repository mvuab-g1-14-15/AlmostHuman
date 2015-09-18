#include "AnimatedModelsManager.h"
#include "AnimatedCoreModel.h"
#include "AnimatedInstanceModel.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "EngineManagers.h"
#include "Utils\BaseUtils.h"

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
    CXMLTreeNode lFile, lNode;

    if( lFile.LoadAndFindNode( mConfigPath.c_str(),"animated_models", lNode ) )
    {
		std::vector<CAnimatedCoreModel*> lACM;
		//TIMER_START();
        for ( uint32 i = 0, lCount = lNode.GetNumChildren(); i < lCount; ++i )
        {
            const CXMLTreeNode& lCurrentNode = lNode(i);
            const std::string& name = lCurrentNode.GetAttribute<std::string>( "name", "no_name" );
            const std::string& path = lCurrentNode.GetAttribute<std::string>( "path", "no_file" );

            CAnimatedCoreModel* lAnimatedCoreModel = new CAnimatedCoreModel( name );

            if( lAnimatedCoreModel->Load( path ) && AddResource( name, lAnimatedCoreModel ) )
            {
				lACM.push_back(lAnimatedCoreModel);
            }
            else
            {
                LOG_ERROR_APPLICATION( "The animated core model %s could not be loaded", name.c_str() );
                CHECKED_DELETE( lAnimatedCoreModel );
            }
        }
		//TIMER_STOP( "Animated models." );

		int lSize( lACM.size() );
		TIMER_START();
		#pragma omp parallel for shared(lSize)
		for(int i = 0; i < lSize; ++i)
			lACM[i]->LoadVertexBuffer( GraphicsInstance );
		TIMER_STOP( "Animated models. LoadVertexBuffer." );
    }
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