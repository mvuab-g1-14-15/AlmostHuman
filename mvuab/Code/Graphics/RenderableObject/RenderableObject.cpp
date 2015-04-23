#include "RenderableObject.h"

#include "XML\XMLTreeNode.h"

CRenderableObject::CRenderableObject() : CObject3D(), CName()
{
}

CRenderableObject::CRenderableObject(CXMLTreeNode &atts) : CObject3D(atts), CName(atts.GetPszProperty("name", ""))
{
}

CRenderableObject::~CRenderableObject()
{
}

