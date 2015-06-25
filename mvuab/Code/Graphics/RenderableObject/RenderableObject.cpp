#include "RenderableObject.h"

#include "XML\XMLTreeNode.h"

CRenderableObject::CRenderableObject() : CObject3D(), CName()
{
}

CRenderableObject::CRenderableObject(const CXMLTreeNode &atts)
    : CObject3D(atts)
    , CName(atts.GetAttribute<std::string>("name", ""))
{
}

CRenderableObject::~CRenderableObject()
{
}

