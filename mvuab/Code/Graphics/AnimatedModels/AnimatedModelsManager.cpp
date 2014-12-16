#include "AnimatedModelsManager.h"
#include "AnimatedCoreModel.h"
#include "AnimatedInstanceModel.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"

CAnimatedModelsManager::CAnimatedModelsManager()
{
	CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);
}

CAnimatedModelsManager::~CAnimatedModelsManager()
{
    Destroy();
}

CAnimatedCoreModel * CAnimatedModelsManager::GetCore(const std::string &Name, const std::string &Path)
{
    CAnimatedCoreModel * l_pAnimatedCoreModel = GetResource(Name);

    // Check if the core model is already in memory
    if(!l_pAnimatedCoreModel)
        l_pAnimatedCoreModel = AddNewCore(Name,Path);
    
    assert(l_pAnimatedCoreModel);

    return l_pAnimatedCoreModel;
}

CAnimatedCoreModel * CAnimatedModelsManager::GetCore(const std::string &Name)
{
  CAnimatedCoreModel * l_pAnimatedCoreModel = GetResource(Name);

  assert(l_pAnimatedCoreModel);

  return l_pAnimatedCoreModel;
}

CAnimatedInstanceModel * CAnimatedModelsManager::GetInstance(const std::string &Name)
{
    return 0;
}

void CAnimatedModelsManager::Load(const std::string &Filename)
{
    m_FileName = Filename;
    
    CXMLTreeNode newFile;
    if (!newFile.LoadFile(m_FileName.c_str()))
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CAnimatedModelManager::Load No se puede abrir \"%s\"!", m_FileName.c_str());
        return;
    }

    CXMLTreeNode node = newFile["animated_models"];
    if(!node.Exists())
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CAnimatedModelManager::Load Tag \"%s\" no existe",  "animated_models");
        return;
    }

    for(int32 i = 0; i < node.GetNumChildren(); ++i)
    {
        const std::string &name = node(i).GetPszProperty("name", "no_name");
        const std::string &path = node(i).GetPszProperty("path", "no_file");
        AddNewCore(name,path);
    }
}

CAnimatedCoreModel* CAnimatedModelsManager::AddNewCore( const std::string &Name, const std::string &Path )
{
   CAnimatedCoreModel *l_pAnimatedCoreModel = new CAnimatedCoreModel(Name);
   if(!l_pAnimatedCoreModel->Load(Path))
    {
        CHECKED_DELETE(l_pAnimatedCoreModel);
    }
    else
        AddResource(Name, l_pAnimatedCoreModel); 

   return l_pAnimatedCoreModel;
}