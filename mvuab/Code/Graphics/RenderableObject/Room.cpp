#include "Room.h"
#include "Utils\Defines.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"
#include "Lights\LightProbe.h"

#include <algorithm>

CRoom::CRoom()
	: m_Name( "" )
	, m_BasePath( "" )
	, m_RenderableObjectsPath( "" )
	, m_StaticMeshesPath( "" )
	, m_pLayers( 0 )
	, m_Active( false )
{
}

CRoom::~CRoom() 
{ 
	CHECKED_DELETE(m_pLayers);

    for(unsigned int i = 0; i < mLightProbes.size(); i++)
    {
        CHECKED_DELETE(mLightProbes[i]);
    }
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
				if (lLightProbe)
					mLightProbes.push_back( lLightProbe );
            }
		}
	}
}

struct SPointDist
{
	CLightProbe* lightprobe;
	float distance;
};

bool SPointDistComparison( SPointDist a, SPointDist b)
{
	return a.distance < b.distance;
};

std::vector<CLightProbe*> CRoom::GetClosedLightProbes( Math::Vect3f aPos )
{
	std::vector<CLightProbe*> lLightProbes;

  if (!mLightProbes.empty())
  {
	  std::vector<SPointDist> lDistances;
	  for( unsigned int i = 0; i < mLightProbes.size(); ++i)
	  {
		  Math::Vect3f lPos = mLightProbes[i]->GetPosition();
		  float lDist = lPos.SqDistance( aPos );
		  SPointDist s = { mLightProbes[i], lDist };
		  lDistances.push_back( s );
	  }

	  std::sort( lDistances.begin(), lDistances.end(), SPointDistComparison);

	  for (int i = 0; i < 4; ++i)
		  lLightProbes.push_back(lDistances[i].lightprobe);
  }

	return lLightProbes;
}