#pragma once
#ifndef INC_INSTANCE_MESH_H_
#define INC_INSTANCE_MESH_H_

#include "RenderableObject\RenderableObject.h"
#include "Utils\Defines.h"

#include <string>

class CStaticMesh;
class CGraphicsManager;
class CXMLTreeNode;

class CInstanceMesh : public CRenderableObject
{
    private:
        CStaticMesh *m_pStaticMesh;

    public:
        CInstanceMesh(const std::string& Name, const std::string &CoreName);
        CInstanceMesh(CXMLTreeNode& atts);
        ~CInstanceMesh();
        
        void Render();
        GET_SET(CStaticMesh*, pStaticMesh);
};

#endif //INC_INSTANCE_MESH_H_
