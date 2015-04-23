#include "RenderableObjectsLayerManager.h"
#include "RenderableObjectsManager.h"
#include "Camera\Object3D.h"
#include "Named.h"
#include "UABVisible.h"
#include "StaticMeshes\MeshInstance.h"
#include "RenderableObject.h"
#include <string.h>
#include "GraphicsManager.h"
#include "StaticMeshes\StaticMeshManager.h"
#include "XML/XMLTreeNode.h"
#include "Utils\Logger\Logger.h"
#include "AnimatedModels\AnimatedInstanceModel.h"
#include "AnimatedModels\AnimatedModelManager.h"
#include "Utils\Exception\Exception.h"
#include "RenderableObject.h"
#if defined( _DEBUG )
	#include "Utils\MeamLeaks.h"
#endif

CRenderableObjectsLayersManager::CRenderableObjectsLayersManager():
m_FileName("")
{
}
CRenderableObjectsLayersManager::~CRenderableObjectsLayersManager()
{
	Unload();
	Destroy();
}
void CRenderableObjectsLayersManager::Update(float ElapsedTime)
{
	for (u_int i = 0; i < m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Update(ElapsedTime);
	}
}

void CRenderableObjectsLayersManager::Render(CGraphicsManager *GM)
{
	for (u_int i = 0; i < m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Render(GM);
	}
}
void CRenderableObjectsLayersManager::Render(CGraphicsManager *GM,const std::string &LayerName)
{
	CRenderableObjectsManager *l_RenderableObjectsManager = GetResource(LayerName);
	if (l_RenderableObjectsManager != NULL)
		l_RenderableObjectsManager->Render(GM);
}

void CRenderableObjectsLayersManager::Load(const std::string &FileName)
{
	m_FileName = FileName;
	CXMLTreeNode NewXML;
	std::string nameProperty="";

	if (NewXML.LoadFile(FileName.c_str()))
	{
		CXMLTreeNode  m = NewXML["renderable_objects"];
		int count = m.GetNumChildren();
		std::string l_DefaultLayer;
		for (int i = 0; i < count; ++i)
		{
			nameProperty = m(i).GetName();
			if (nameProperty == "layer")
			{
				std::string name = m(i).GetPszProperty("name");
				std::string name_transferred = m(i).GetPszProperty("name");
				name_transferred += "_transferred";
				bool l_Default = m(i).GetBoolProperty("default");
				CRenderableObjectsManager *l_ObjectManager = new CRenderableObjectsManager(name);
				if(!AddResource(name, l_ObjectManager))
				{
					CHECKED_DELETE(l_ObjectManager);
				}else
				{
					if (l_Default)
					{
						m_DefaultRenderableObjectManager = l_ObjectManager;
						l_DefaultLayer = name;
					}
				}
				CRenderableObjectsManager *l_ObjectTransferredManager = new CRenderableObjectsManager(name_transferred);
				if(!AddResource(name_transferred, l_ObjectTransferredManager))
				{
					CHECKED_DELETE(l_ObjectTransferredManager);
				}
				
			}else if (nameProperty == "mesh_instance")
			{
 				std::string layer = m(i).GetPszProperty("layer",l_DefaultLayer.c_str());
				std::string name = m(i).GetPszProperty("name");
				std::string core = m(i).GetPszProperty("core");
				std::string technique = m(i).GetPszProperty("technique","Default");
				if (technique =="Default")
				{	
					technique = "null";
					//LOGGER->AddNewLog(ELL_WARNING, "Technique = %s", technique);
				}else{
					std::string technique = m(i).GetPszProperty("technique","default_tecnique");
					//LOGGER->AddNewLog(ELL_WARNING, "Technique = %s", technique);
				}
	
				CRenderableObjectsManager *l_ObjectManager = GetResource(layer);
				if (l_ObjectManager != NULL)
				{
					CMeshInstance *l_Mesh = new CMeshInstance(m(i));
					l_Mesh->SetStaticMesh(CORE->GetStaticMeshManager()->GetResource(core));
					if(!l_ObjectManager->AddResource(name, l_Mesh))
					{
						LOGGER->AddNewLog(ELL_ERROR,"Renderable Object duplicated: %s", name.c_str());
					}
				}
			}
			else if (nameProperty == "animated_model")
			{
 				std::string layer = m(i).GetPszProperty("layer",l_DefaultLayer.c_str());
				std::string name = m(i).GetPszProperty("name");
				std::string core = m(i).GetPszProperty("core");
					CRenderableObjectsManager *l_ObjectManager = GetResource(layer);
					CAnimatedCoreModel *l_AnimatedCoreModel = CORE->GetAnimatedModelManager()->GetResource(core);
					if (l_AnimatedCoreModel != NULL && l_ObjectManager->GetResource(name) == NULL)
					{
						CAnimatedInstanceModel *l_AnimatedInstanceModel = new CAnimatedInstanceModel(m(i));
						l_AnimatedInstanceModel->Initialize(l_AnimatedCoreModel, CORE->GetGraphicsManager());
						l_ObjectManager->AddResource(name, l_AnimatedInstanceModel);
					}
			}
		}
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "RenderableObjectManager::Load->Error al intentar leer el archivo: %s", FileName.c_str());
		std::string l_msgerror = "Error, se genero un logger con la informacion";
		throw CException(__FILE__, __LINE__, l_msgerror);
	}

}

void CRenderableObjectsLayersManager::Reload()
{
	if (m_FileName != "")
	{
		Unload();
		Destroy();
		Load(m_FileName);
	}
}
void CRenderableObjectsLayersManager::Unload()
{ 
	for (u_int i = 0; i < m_ResourcesVector.size(); ++i)
	{
		m_ResourcesVector[i]->Destroy();
	}	
}



