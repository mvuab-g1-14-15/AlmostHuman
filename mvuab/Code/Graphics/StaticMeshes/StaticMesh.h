#pragma once
#ifndef INC_STATIC_MESH_H_
#define INC_STATIC_MESH_H_

#include <vector>
#include <string>

#include "Math/AABB.h"

class CRenderableVertexs;
class CGraphicsManager;
class CTexture;

class CStaticMesh
{
protected:
  typedef std::vector<CTexture *> TTextureVector;
  std::vector<CRenderableVertexs *> m_RVs;
  std::vector<TTextureVector> m_Textures;

  std::string m_FileName;

  unsigned int m_NumVertexs;
  unsigned int m_NumFaces;
  unsigned int m_NumDraws;

  AABB3f m_AABB;

public:
  CStaticMesh();
  ~CStaticMesh();

  bool Load            (const std::string &FileName);
  bool ReLoad        ();

  void Render        (CGraphicsManager *GM);
  void Destroy        ();

  unsigned int        GetNumVertex    () { return m_NumVertexs; }
  unsigned int        GetNumFaces        () { return m_NumFaces; }
  unsigned int        GetNumDraws        () { return m_NumDraws; }

  AABB3f GetAABB() { return m_AABB;}

};

#endif //INC_STATIC_MESH_H_