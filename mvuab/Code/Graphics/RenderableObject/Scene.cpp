#include "Scene.h"
#include "Room.h"

#include "Utils\Defines.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"

CSene::CSene()
{

}

CSene::~CSene()
{
    CMapManager::Destroy();
}

bool CSene::Load(const std::string &l_FilePath)
{
    CXMLTreeNode l_Root, l_Node;
    if(!l_Root.LoadAndFindNode(l_FilePath.c_str(), "room", l_Node))
    {
        return false;
    }

    for (int i = 0, l_NumChilds = l_Node.GetNumChildren(); i < l_NumChilds; ++i)
    {
        CRoom *l_Room = new CRoom();
        CXMLTreeNode& l_CurrentNode = l_Node(i);

        const std::string &l_Path = l_CurrentNode.GetAttribute<std::string>("path", "no_path");
        const std::string &l_File = l_CurrentNode.GetAttribute<std::string>("file", "no_file");
        const std::string &l_Level = l_CurrentNode.GetAttribute<std::string>("level", "no_level");

        l_Room->Load(l_Path + "/" + l_File, l_Level);
        if(!AddResource(l_Level, l_Room)) CHECKED_DELETE(l_Room);     
    }

    return true;
}

