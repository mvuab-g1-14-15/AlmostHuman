#include "LensFlare.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "EngineManagers.h"
#include "Utils/Defines.h"
#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"

//---------------------------------------------------------------------------------
CLensFlare::CLensFlare()
    : mTechnique(0)
{
}

//---------------------------------------------------------------------------------
CLensFlare::~CLensFlare()
{
    for(size_t i = 0; i < mFlares.size(); ++i)
    {
        CHECKED_DELETE(mFlares.at(i));
    }
    mFlares.clear();
}

//---------------------------------------------------------------------------------
bool CLensFlare::Init( const CXMLTreeNode& node )
{
    bool lOk = true;
    SetName( node.GetPszProperty("name") );
    mTechnique = EffectManagerInstance->GetEffectTechnique(node.GetPszProperty("technique"));

    ASSERT(mTechnique, "Null technique to render the flares");
    for( uint32 i = 0, lCount = node.GetNumChildren(); i < lCount; ++i )
    {
        const CXMLTreeNode& lNode       = node(i);
        const std::string & lNodeName   = lNode.GetName();
        if( lNodeName == "flare" )
        {
            CFlare* lFlare = new CFlare();
            if( lFlare->Init( lNode ) )
            {
                mFlares.push_back( lFlare );
            }
            else
            {
                CHECKED_DELETE( lFlare );
            }
        }
    }
    return true;
}

CFlare::CFlare()
{
}

CFlare::~CFlare()
{
}

bool CFlare::Init( const CXMLTreeNode& node )
{
    mColor   = node.GetVect3fProperty("color", Math::Vect3f());
    mSize    = node.GetVect2fProperty("size", Math::Vect2f() );
    mPos     = node.GetAttribute<float>("position", 0.0f);
    mTexture = TextureMInstance->GetTexture( node.GetPszProperty("texture") );
    return true;;
}

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

    if( lFile.LoadAndFindNode( mConfigPath.c_str(), "flares", lLensFlare ) )
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

}