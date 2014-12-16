#pragma once
#ifndef INC_INSTANCE_MESH_H_
#define INC_INSTANCE_MESH_H_

#include "RenderableObject\RenderableObject.h"
#include "Utils\Defines.h"

#include <string>

class CStaticMesh;
class CGraphicsManager;

class CInstanceMesh : public CRenderableObject
{
private:
    CStaticMesh* m_pStaticMesh;

    unsigned int m_NumVertexs;
    unsigned int m_NumFaces;
    unsigned int m_NumDraws;

public:
    CInstanceMesh(const std::string &Name, const std::string &CoreName);
    ~CInstanceMesh();

    void Render();
    GET_SET(CStaticMesh*, pStaticMesh);

    unsigned int GetNumVertex    () { return m_NumVertexs; }
    unsigned int GetNumFaces    () { return m_NumFaces; }
    unsigned int GetNumDraws    () { return m_NumDraws; }
};

#endif //INC_INSTANCE_MESH_H_
