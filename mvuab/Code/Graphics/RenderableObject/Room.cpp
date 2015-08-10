#include "Room.h"
#include "Utils\Defines.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "Lights\LightProbe.h"

CRoom::CRoom()
	: m_Name( "" )
	, m_BasePath( "" )
	, m_RenderableObjectsPath( "" )
	, m_StaticMeshesPath( "" )
	, m_pLayers( 0 )
	, m_Active( false )
{
}

CRoom::~CRoom() { 
	CHECKED_DELETE(m_pLayers);
}

void CRoom::LoadLightProbe()
{
	bool lOk = false;
    if ( m_BasePath != "" )
    {
		std::string lPath = m_BasePath + "light_probe.xml";
        CXMLTreeNode l_Root, l_Node;
        if ( l_Root.LoadAndFindNode( lPath.c_str(), "light_probe_room", l_Node ) )
        {
            for ( int i = 0, l_NumChilds = l_Node.GetNumChildren(); i < l_NumChilds; ++i )
            {
                CXMLTreeNode& l_CurrentNode = l_Node( i );

				CLightProbe* lLightProbe = new CLightProbe( l_CurrentNode );
            }
		}
	}
}