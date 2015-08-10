#ifndef LIGHT_PROBE_H
#define LIGHT_PROBE_H

#include "Math\Vector3.h"
#include "Math\Vector2.h"

#include <map>

class CXMLTreeNode;

struct SLightProbeVertex
{
	Math::Vect3f position;
	Math::Vect2f uv;
};

class CLightProbe
{
public:
	CLightProbe( const CXMLTreeNode& );
	~CLightProbe();

	Math::Vect3f mPosition;

	std::map<std::string, SLightProbeVertex> mVertexs;
};

#endif // LIGHT_PROBE_H
