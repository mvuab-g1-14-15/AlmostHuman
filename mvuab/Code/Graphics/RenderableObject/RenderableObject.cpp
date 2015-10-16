#include "RenderableObject.h"

#include "XML\XMLTreeNode.h"

CRenderableObject::CRenderableObject() : CObject3D(), CName()
{
}

CRenderableObject::CRenderableObject(const std::string & aName )
    : CName( aName )
    , CObject3D()
{
}

CRenderableObject::CRenderableObject(const CXMLTreeNode &atts)
    : CObject3D(atts)
    , CName(atts.GetAttribute<std::string>("name", ""))
    , m_Active( atts.GetAttribute<bool>("active", true ))
    , m_RoomName( "" )
{
}

CRenderableObject::~CRenderableObject()
{
}

std::string
CRenderableObject::GetRoomName()
{
  return m_RoomName;
}

void 
CRenderableObject::SetRoomName( std::string a_Name )
{
  m_RoomName = a_Name;
}