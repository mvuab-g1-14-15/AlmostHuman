#ifndef ANIMATED_CORE_MODEL_H
#define ANIMATED_CORE_MODEL_H
#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

#include "Utils\Name.h"

// Define the default xml file that will be searched inside the path
const std::string defaultXML = "actor.xml";

class CRenderableVertexs;
class CalHardwareModel;
class CGraphicsManager;
class CalCoreModel;
class CTexture;
class CXMLTreeNode;

class CAnimatedCoreModel : public CName
{
public:
    struct SAnimation {
        std::string mName;
        int iId;
        float fWeight;
        bool bFromParameter, bFromComplementaryParameter;
        float fFadeOnChange;

        SAnimation():iId(0),fWeight(0),bFromParameter(false),bFromComplementaryParameter(false),fFadeOnChange(0) {};
        bool operator <(const SAnimation& _other) const { return iId < _other.iId; };
    };

    struct SCycle: SAnimation {
    };

    struct SAction: SAnimation
    {
        bool bBlock;
        bool bStop;
        float fFadeIn;
        float fFadeOut;

        SAction():bBlock(false),bStop(false),fFadeIn(.3f),fFadeOut(.3f){};
    };

    struct SAnimationState {
        std::string mName;
        float fDefaultFadeIn, fDefaultFadeOut;
        std::set<SCycle> Cycles;
        std::set<SAction> OnEnter;
        std::set<SAction> OnExit;

        SAnimationState():fDefaultFadeIn(0),fDefaultFadeOut(0){};
    };

    struct SAnimationChange {
        float fFade;
        std::set<SAction> Actions;

        SAnimationChange():fFade(0){}
    };

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

  void ReloadAnimationStates();

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
  bool LoadAnimation( const std::string& Name, const std::string& Filename );
  void LoadTextures();

  std::string m_szDefaultAnimationState;
  std::map<std::string, SAnimationState>               m_AnimationStates;
  std::map<std::string, std::map<std::string, SAnimationChange>> m_AnimationChanges;

  friend class CAnimatedInstanceModel;

  void LoadAnimationStates( const CXMLTreeNode& _xmlAnimationStates);
  void LoadAnimationChanges( const CXMLTreeNode& _xmlAnimationChanges);
  bool LoadAnimationFromState( const CXMLTreeNode &_xmlAnimation, CAnimatedCoreModel::SAnimation &Animation_);
  bool LoadActionFromState( const CXMLTreeNode &_xmlAction, CAnimatedCoreModel::SAction &Action_);
  bool LoadAnimationState( const CXMLTreeNode &_xmlAnimationState, CAnimatedCoreModel::SAnimationState &AnimationState_);
  bool LoadAnimationChange( const CXMLTreeNode &_xmlAnimationChange, CAnimatedCoreModel::SAnimationChange &AnimationChange_);
};

#endif