#include "Layer.h"
#include "Room.h"

#include "Utils\Defines.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"

CLayer::CLayer()
{

}

CLayer::~CLayer()
{

}

bool CLayer::Load(const std::string &l_FilePath)
{
    CXMLTreeNode l_Root, l_Node;
    if(!l_Root.LoadAndFindNode(l_FilePath.c_str(), "RenderableObjects", l_Node))
    {
        return false;
    }

    for (int i = 0, l_NumChilds = l_Node.GetNumChildren(); i < l_NumChilds; ++i)
    {
        
     
    }

    return true;
}

