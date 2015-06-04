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
        const CXMLTreeNode& lNode		= node(i);
        const std::string & lNodeName	= lNode.GetName();
        if( lNodeName == "flare" )
        {
			CFlare* lFlare = new CFlare();
			if( lFlare->Init( lNode ) )
				mFlares.push_back( lFlare );
			else
				CHECKED_DELETE( lFlare );
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
	return false;
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

}

void CLensFlareManager::Update()
{

}

void CLensFlareManager::Render()
{

}