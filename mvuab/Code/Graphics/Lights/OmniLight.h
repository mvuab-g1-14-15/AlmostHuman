#ifndef OMNI_LIGHT_H
#define OMNI_LIGHT_H

#include "Light.h"

#include "XML\XMLTreeNode.h"

class COmniLight : public CLight
{
public:
	COmniLight(CXMLTreeNode node);
};

#endif // OMNI_LIGHT_H
