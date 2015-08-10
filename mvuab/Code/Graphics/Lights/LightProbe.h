#ifndef LIGHT_PROBE_H
#define LIGHT_PROBE_H

#include "Math\Vector3.h"
#include "Math\Vector2.h"

#include <map>

class CXMLTreeNode;

class CLightProbeVertex
{
public:
	CLightProbeVertex( Math::Vect3f a_position, Math::Vect2f a_uv)
		: mPosition( a_position )
		, mUV( a_uv )
	{
	}
	~CLightProbeVertex()
	{}

	Math::Vect3f GetPosition() { return mPosition; }
	Math::Vect2f GetUV() { return mUV; }

private:
	Math::Vect3f mPosition;
	Math::Vect2f mUV;
};

class CLightProbe
{
public:
	CLightProbe( const CXMLTreeNode& );
	~CLightProbe();

	Math::Vect3f GetPosition() { return mPosition; }

private:
	Math::Vect3f mPosition;

	std::map<std::string, CLightProbeVertex*> mVertexs;
};

#endif // LIGHT_PROBE_H
