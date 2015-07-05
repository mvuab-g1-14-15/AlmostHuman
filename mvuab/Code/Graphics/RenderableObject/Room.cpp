#include "Room.h"
#include "Utils\Defines.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"


CRoom::CRoom() : CName()
{

}

CRoom::~CRoom()
{

}

bool CRoom::Load(const std::string &l_FilePath, const std::string &l_RoomName)
{
    CXMLTreeNode l_Node, l_Root;
    SetName(l_RoomName);

    if(!l_Root.LoadAndFindNode(l_FilePath.c_str(), "place_node_here", l_Node))
    {
        return false;
    }

    for (int i = 0, l_NumChilds = l_Node.GetNumChildren(); i < l_NumChilds; ++i)
    {

    }

    return true;
}

