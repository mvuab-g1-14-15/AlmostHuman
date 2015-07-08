#include "Room.h"
#include "Utils\Defines.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"
#include "Layer.h"


CRoom::CRoom() : CName()
{

}

CRoom::~CRoom()
{
   CMapManager::Destroy();
}

bool CRoom::Load(const std::string &l_FilePath, const std::string &l_RoomName)
{
    CXMLTreeNode l_Node, l_Root;
    SetName(l_RoomName);

    if(!l_Root.LoadAndFindNode(l_FilePath.c_str(), "room", l_Node))
    {
        return false;
    }

    const std::string &l_Path = l_Root.GetAttribute<std::string>("path", "no_path");

    for (int i = 0, l_NumChilds = l_Node.GetNumChildren(); i < l_NumChilds; ++i)
    {
      CLayer *l_Layer = new CLayer();
      CXMLTreeNode& l_CurrentNode = l_Node(i);

      const std::string &l_Name = l_CurrentNode.GetAttribute<std::string>("name", "no_path");
      const std::string &l_File = l_CurrentNode.GetAttribute<std::string>("file", "no_file");
      

      l_Layer->Load(l_Path + "/" + l_File);
      if(!AddResource(l_Name, l_Layer)) CHECKED_DELETE(l_Layer);     

    }

    return true;
}

