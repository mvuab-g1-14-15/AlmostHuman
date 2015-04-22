#pragma once
#ifndef RENDERABLE_OBJECT_LAYERS_MANAGER_H
#define RENDERABLE_OBJECT_LAYERS_MANAGER_H

#include "RenderableObject\RenderableObjectsManager.h"
#include "Utils\TemplatedVectorMapManager.h"
#include "XML\XMLTreeNode.h"
#include "Utils\Manager.h"

class CGraphicsManager;

class CRenderableObjectsLayersManager : public CTemplatedVectorMapManager<CRenderableObjectsManager>, public CManager
{
    private:
        CRenderableObjectsManager* m_DefaultRenderableObjectManager;
        CRenderableObjectsManager* GetRenderableObjectManager( CXMLTreeNode& Node );

    public:
        CRenderableObjectsLayersManager();
        CRenderableObjectsLayersManager( CXMLTreeNode& atts);
        ~CRenderableObjectsLayersManager();

        
        void Init();
        void Reload();
        void Destroy();

        void Update();
        void Render();
        void Render( const std::string& LayerName );
};
#endif