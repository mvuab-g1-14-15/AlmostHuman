#pragma once
#ifndef RENDERABLE_OBJECT_LAYERS_MANAGER_H
#define RENDERABLE_OBJECT_LAYERS_MANAGER_H

#include "RenderableObject\RenderableObjectsManager.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "XML\XMLTreeNode.h"
#include "Utils\Manager.h"

class CGraphicsManager;
class CPhysicUserData;

class CRenderableObjectsLayersManager : public CTemplatedVectorMapManager<CRenderableObjectsManager>
{
    private:
        std::vector<CPhysicUserData *> m_PhyscsUserData;
        CRenderableObjectsManager* m_DefaultRenderableObjectManager;
        CRenderableObjectsManager* GetRenderableObjectManager( const std::string &l_Name );
		void LoadRenderableObject(const std::string &l_FilePath, const std::string& l_Name, const std::string& l_RoomName);
		
        void AddNewInstaceMesh(const CXMLTreeNode& atts, const std::string &l_Layer, const std::string &l_RoomName);

    public:
        CRenderableObjectsLayersManager();
        CRenderableObjectsLayersManager(const  CXMLTreeNode& atts);
        ~CRenderableObjectsLayersManager();

        void Destroy();
        
		bool LoadLayers(const std::string &l_FilePath, const std::string &l_RoomName);
        void Update();

        void Render();
        void Render( const std::string& LayerName );
};
#endif