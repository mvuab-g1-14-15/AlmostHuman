#include "LensFlare/LensFlareManager.h"
#include "LensFlare/LensFlare.h"
#include "XML/XMLTreeNode.h"
#include "Lights/LightManager.h"
#include "Lights/Light.h"
#include "GraphicsManager.h"
#include "Utils/Defines.h"
#include "EngineManagers.h"

CLensFlareManager::CLensFlareManager(const CXMLTreeNode &atts)
    : CManager( atts )
{
}

CLensFlareManager::~CLensFlareManager()
{

}

void CLensFlareManager::Init()
{
    CXMLTreeNode lLensFlare, lFile;

    if( lFile.LoadAndFindNode( mConfigPath.c_str(), "lens_flares", lLensFlare ) )
    {
        for( uint32 i = 0, lCount = lLensFlare.GetNumChildren(); i < lCount; ++i )
        {
            const CXMLTreeNode& lNode = lLensFlare(i);
            const std::string& lTagName = lNode.GetName();
            if( lTagName == "lens_flare" )
            {
                CLensFlare* lFlare = new CLensFlare();
                if( lFlare->Init( lNode ) )
                {
                    if(!AddResource( lFlare->GetName(), lFlare ))
                    {
                        CHECKED_DELETE( lFlare );
                    }
                }
                else
                {
                    LOG_WARNING_APPLICATION("Could not load flare %s", lFlare->GetName() );
                    CHECKED_DELETE( lFlare );
                }
            }
        }
    }
}

void CLensFlareManager::Update()
{

}

void CLensFlareManager::Render()
{
    CGraphicsManager* lGM = GraphicsInstance;
    uint32 lWidth, lHeight;
    lGM->GetWidthAndHeight(lWidth, lHeight);

    float lAspectRatio = float(lWidth / lHeight);

    CLightManager* lLM = LightMInstance;

    for( uint32 i = 0, lCount = lLM->GetLightCount(); i < lCount ; ++i )
    {
        CLight* lCurrentLight = lLM->GetLight(i);

        CLensFlare* lLensFlare = lCurrentLight->GetLensFlare();

        if( lLensFlare )
        {
            Math::Vect2f lLightProj = lGM->ToScreenCoordinates(lCurrentLight->GetPosition());
            lLensFlare->Render( Math::Vect2u( uint32(lLightProj.x), uint32(lLightProj.y )), Math::Vect2u( lWidth, lHeight ), lAspectRatio );
        }
    }
}