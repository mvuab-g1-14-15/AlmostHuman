#include "LightManager.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "OmniLight.h"
#include "SpotLight.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "EngineConfig.h"
#include "LensFlare.h"

CLightManager::CLightManager()
    : CManager()
{
}

CLightManager::CLightManager(CXMLTreeNode& atts)
    : CManager(atts)
    , mAmbientLightColor( Math::Vect3f( 0.0f, 0.0f, 0.0f) )
{

}

CLightManager::~CLightManager()
{
}

void CLightManager::Init()
{
    //mConfigPath = EngineConfigInstance->GetLightsPath();
    Load(mConfigPath);
}
bool CLightManager::Load( const std::string& FileName )
{
	mConfigPath = FileName;

    CXMLTreeNode newFile;

	bool lOk = false;

    if ( newFile.LoadFile( FileName.c_str() ) )
    {
        CXMLTreeNode m = newFile["lights"];

		if ( m.Exists() )
		{
			for ( uint32 i = 0, lCount = m.GetNumChildren() ; i < lCount; ++i )
			{
				const CXMLTreeNode& lCurrentNode = m(i);
				const std::string & lCurrentNodeName = lCurrentNode.GetName();

				CLight* lNewLight = 0;

				if ( lCurrentNodeName == "omni_light" )
				{
					lNewLight = new COmniLight(lCurrentNode);
				}
				else if ( lCurrentNodeName == "directional_light" )
				{
					lNewLight = new CDirectionalLight(lCurrentNode);
				}
				else if ( lCurrentNodeName == "spot_light" )
				{
					lNewLight = new CSpotLight(lCurrentNode);
				}
				else if( lCurrentNodeName == "ambient_light" )
				{
					mAmbientLightColor = lCurrentNode.GetVect3fProperty( "color", Math::Vect3f( 0.0f, 0.0f, 0.0f ) );
				}
				else if( lCurrentNodeName == "lens_flare" )
				{
					CLensFlare* lLensFlare = new CLensFlare();
					if( lLensFlare->Init( lCurrentNode ) )
					{
						mLensFlares.AddResource( lLensFlare->GetName(), lLensFlare );
					}
					else
					{
						CHECKED_DELETE(lLensFlare);
						LOG_ERROR_APPLICATION( "Error creating lens flare %s", lLensFlare->GetName().c_str() );
					}
				}

				if ( lNewLight && !AddResource( lNewLight->GetName(), lNewLight ) )
				{
					CHECKED_DELETE( lNewLight );
				}
			}
		}
	}

	if( !lOk )
		LOG_ERROR_APPLICATION( "Error loading Lights %s.", FileName.c_str() );

    return lOk;;
}

void CLightManager::Render()
{
    #ifdef _DEBUG
    TVectorResources::iterator itb = m_ResourcesVector.begin(), ite = m_ResourcesVector.end();

    for ( ; itb != ite; ++itb )
    {
        ( *itb )->Render();
    }

    #endif
}

CLight* CLightManager::GetLight( size_t at )
{
    return GetResourceById( at );
}

size_t  CLightManager::GetLightCount()
{
    return m_ResourcesVector.size();
}

void CLightManager::GenerateShadowMap( CGraphicsManager* GM )
{
    TVectorResources::iterator itb = m_ResourcesVector.begin(), ite = m_ResourcesVector.end();

    for ( ; itb != ite; ++itb )
    {
        ( *itb )->GenerateShadowMap( GM );
    }
}

bool CLightManager::ReLoad()
{
    Destroy();
    return Load( mConfigPath );
}