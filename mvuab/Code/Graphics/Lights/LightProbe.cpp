#include "LightProbe.h"
#include "Utils\Defines.h"
#include "XML\XMLTreeNode.h"

CLightProbe::CLightProbe( const CXMLTreeNode& aXMLNode )
{
    for ( int i = 0, l_NumChilds = aXMLNode.GetNumChildren(); i < l_NumChilds; ++i )
    {
        CXMLTreeNode& l_CurrentNode = aXMLNode( i );
		std::string lTagName( l_CurrentNode.GetName() );

		if (lTagName == "center")
			mPosition = l_CurrentNode.GetAttribute<Math::Vect3f>( "pos", Math::Vect3f(0.0f) );
		if (lTagName == "vertexs")
		{
			for ( int j = 0, l_NumVertexs = l_CurrentNode.GetNumChildren(); j < l_NumVertexs; ++j )
			{
				CXMLTreeNode l_VertexNode = l_CurrentNode( j );
				std::string lVertexTag( l_VertexNode.GetName() );

				if (lVertexTag == "vertex")
				{
					Math::Vect3f lPos( l_VertexNode.GetAttribute<Math::Vect3f>( "pos", Math::Vect3f(0.0f) ));
					Math::Vect2f lUV( l_VertexNode.GetAttribute<Math::Vect2f>( "uv", Math::Vect2f(0.0f) ));

					Math::Vect3f lDir( lPos - mPosition );
					lDir.Normalize();

					CLightProbeVertex* v = new CLightProbeVertex(lPos, lUV);

					std::string lKey( "" );
					if (lDir.x == 1.0f)
						lKey = "x";
					if (lDir.x == -1.0f)
						lKey = "-x";
					if (lDir.y == 1.0f)
						lKey = "y";
					if (lDir.y == -1.0f)
						lKey = "-y";
					if (lDir.z == 1.0f)
						lKey = "z";
					if (lDir.z == -1.0f)
						lKey = "-z";

					mVertexs[lKey] = v;
				}
			}
		}
    }
}

CLightProbe::~CLightProbe()
{
    for(std::map<std::string, CLightProbeVertex*>::iterator it = mVertexs.begin(); it != mVertexs.end(); it++)
    {
        CHECKED_DELETE(it->second);
    }

    mVertexs.clear();
}