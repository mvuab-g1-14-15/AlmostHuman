#include "LensFlare.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "EngineManagers.h"
#include "Utils/Defines.h"

//---------------------------------------------------------------------------------
CLensFlare::CLensFlare()
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
	for( uint32 i = 0, lCount = node.GetNumChildren(); i < lCount; ++i )
    {
        const CXMLTreeNode& lNode		= node(i);
        const std::string & lNodeName	= lNode.GetName();
        if( lNodeName == "texture" )
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