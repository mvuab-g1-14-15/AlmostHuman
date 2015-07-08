#include "Scene.h"
#include "RenderableObject\RenderableObjectsLayersManager.h"

#include "Utils\Defines.h"
#include "Logger\Logger.h"
#include "XML\XMLTreeNode.h"


CScene::CScene( const CXMLTreeNode& atts )
	: CManager(atts)
{

}

CScene::~CScene()
{
    Destroy();
}

void CScene::Destroy()
{
	CMapManager::Destroy();
}

void CScene::Init()
{
	Load( mConfigPath );
}

bool CScene::Load(const std::string &l_FilePath)
{
	if (l_FilePath == "")
		return false;
	
    CXMLTreeNode l_Root, l_Node;
    if(!l_Root.LoadAndFindNode(l_FilePath.c_str(), "scene", l_Node))
    {
        return false;
    }

    for (int i = 0, l_NumChilds = l_Node.GetNumChildren(); i < l_NumChilds; ++i)
    {
        CRenderableObjectsLayersManager *l_ROLM = new CRenderableObjectsLayersManager();
		
        CXMLTreeNode& l_CurrentNode = l_Node(i);

        const std::string &l_Path = l_CurrentNode.GetAttribute<std::string>("path", "no_path");
        const std::string &l_File = l_CurrentNode.GetAttribute<std::string>("file", "no_file");
        const std::string &l_Level = l_CurrentNode.GetAttribute<std::string>("level", "no_level");

        l_ROLM->LoadLayers(l_Level, l_Path + "/" + l_File);
        if(!AddResource(l_Level, l_ROLM)) 
			CHECKED_DELETE(l_ROLM);     
    }

    return true;
}

bool CScene::Reload()
{
    CMapManager::Destroy();
	return Load(mConfigPath);
}