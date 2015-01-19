#include "LightManager.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "OmniLight.h"
#include "SpotLight.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"

CLightManager::CLightManager()
{
}

CLightManager::~CLightManager()
{
}

void CLightManager::Load(const std::string &FileName)
{
  CXMLTreeNode newFile;
  CXMLTreeNode m;

  if (!newFile.LoadFile(FileName.c_str()))
  {
    CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR,"CLightManager::Load --> Error loading XML %s.",FileName.c_str());
    return;
  }

  m = newFile["lights"];

  if (!m.Exists())
  {
    CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR,"CLightManager::Load --> Error reading %s, lights no existeix.",FileName.c_str());
    return;
  }

  for(int i=0; i<m.GetNumChildren(); ++i)
  {
    const std::string &l_TagName=m(i).GetName();
    if(l_TagName=="light")
    {
      std::string l_Type = m(i).GetPszProperty("type","");
      if(l_Type=="targetDirect")
      {
        CDirectionalLight* l_Light = new CDirectionalLight(m(i));
        AddResource(l_Light->GetName(), l_Light);
      }
      else if(l_Type=="omni")
      {
        COmniLight* l_Light = new COmniLight(m(i));
		AddResource(l_Light->GetName(), l_Light);
      }
      else if(l_Type=="targetSpot")
      {
        CSpotLight* l_Light = new CSpotLight(m(i));
        AddResource(l_Light->GetName(), l_Light);
      }
    }
  }
}

void CLightManager::Render()
{
}

CLight* CLightManager::GetLight( size_t at )
{
  return GetResourceById(at);
}