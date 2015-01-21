#ifndef OMNI_LIGHT_H
#define OMNI_LIGHT_H

#include "Light.h"

#include "XML\XMLTreeNode.h"

class COmniLight : public CLight
{
public:
  COmniLight( CXMLTreeNode node );
  virtual void Render();
};

#endif // OMNI_LIGHT_H
