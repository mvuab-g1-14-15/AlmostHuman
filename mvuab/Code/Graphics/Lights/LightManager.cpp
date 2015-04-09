#include "LightManager.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "OmniLight.h"
#include "SpotLight.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "EngineConfig.h"
CLightManager::CLightManager()
  : CManager()
{
}

CLightManager::CLightManager(CXMLTreeNode& atts)
  : CManager(atts)
{
	/*TODO RAUL
	PONER LECTURA DE XML
	*/
}

CLightManager::~CLightManager()
{
}

void CLightManager::Init()
{
	mConfigPath = EngineConfigInstance->GetLightsPath();
	Load(mConfigPath);
}
bool CLightManager::Load( const std::string& FileName )
{
  CXMLTreeNode newFile;
  CXMLTreeNode m;

  if ( !newFile.LoadFile( FileName.c_str() ) )
  {
    LOG_ERROR_APPLICATION( "CLightManager::Load --> Error loading XML %s.",
                           FileName.c_str() );
    return false;
  }

  mConfigPath = FileName;
  m = newFile["lights"];

  if ( !m.Exists() )
  {
    LOG_ERROR_APPLICATION(
      "CLightManager::Load --> Error reading %s, lights no existeix.", FileName.c_str() );
    return false;
  }

  for ( int i = 0; i < m.GetNumChildren(); ++i )
  {
    const std::string& l_TagName = m( i ).GetName();

    if ( l_TagName == "light" )
    {
      std::string l_Type = m( i ).GetPszProperty( "type", "" );

      if ( l_Type == "targetDirect" )
      {
        CDirectionalLight* l_Light = new CDirectionalLight( m( i ) );

        if ( !AddResource( l_Light->GetName(), l_Light ) )
          delete l_Light;
      }
      else if ( l_Type == "omni" )
      {
        COmniLight* l_Light = new COmniLight( m( i ) );

        if ( !AddResource( l_Light->GetName(), l_Light ) )
          delete l_Light;
      }
      else if ( l_Type == "targetSpot" )
      {
        CSpotLight* l_Light = new CSpotLight( m( i ) );

        if ( !AddResource( l_Light->GetName(), l_Light ) )
          delete l_Light;
      }
    }
  }

  return true;
}

void CLightManager::Render()
{
#ifdef _DEBUG
  TVectorResources::iterator itb = m_ResourcesVector.begin(), ite = m_ResourcesVector.end();

  for ( ; itb != ite; ++itb )
    ( *itb )->Render();

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
    ( *itb )->GenerateShadowMap( GM );
}

bool CLightManager::ReLoad()
{
  Destroy();
  return Load( mConfigPath );
}