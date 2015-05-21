#ifndef ANIMATED_CORE_MODEL_H
#define ANIMATED_CORE_MODEL_H
#pragma once

#include <string>
#include <vector>
#include <map>

#include "Utils\Name.h"

// Define the default xml file that will be searched inside the path
const std::string defaultXML = "actor.xml";

class CRenderableVertexs;
class CalHardwareModel;
class CGraphicsManager;
class CalCoreModel;
class CTexture;

class CAnimatedCoreModel : public CName
{
public:
  CAnimatedCoreModel( const std::string& Name );
  ~CAnimatedCoreModel();

  CalCoreModel*        GetCoreModel();
  CalHardwareModel*    GetCalHardwareModel();
  CRenderableVertexs*  GetRenderableVertexs();

  const   std::string& GetTextureName( size_t id );
  void    ActivateTextures();

  bool Load( const std::string& Path );
  bool LoadVertexBuffer( CGraphicsManager* RM );

  int     GetAnimationsCount();
  int     GetAnimationId( const std::string& AnimationName ) const;
  size_t  GetNumTextures();

  bool Reload();

protected:
  typedef std::vector<CTexture*> TTextureVector;
  TTextureVector                  m_TextureVector;

  CalCoreModel*                    m_CalCoreModel;
  CalHardwareModel*                m_CalHardwareModel;
  CRenderableVertexs*              m_RenderableVertexs;

  typedef std::map<std::string, uint16> TAnimationsIdMap;
  TAnimationsIdMap                m_AnimationsMap;
  std::string                     m_FileName;
  std::string                     m_Path;

  int                             m_NumVtxs;
  int                             m_NumFaces;

private:
  void Destroy();
  bool Load();

  bool LoadMesh( const std::string& Filename );
  bool LoadSkeleton( const std::string& Filename );
  bool LoadTexture( const std::string& Filename );
  bool LoadAnimation( const std::string& Name, const std::string& Filename );
  void LoadTextures();


};

#endif