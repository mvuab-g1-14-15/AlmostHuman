#include "AnimatedCoreModel.h"
#include "cal3d\cal3d.h"

#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"

#include "Texture\Texture.h"
#include "Texture\TextureManager.h"

#include "GraphicsManager.h"
#include "RenderableVertex\VertexTypes.h"
#include "RenderableVertex\IndexedVertexs.h"
#include "EngineManagers.h"
#include "Utils\BaseUtils.h"

#define MAXBONES 29

CAnimatedCoreModel::CAnimatedCoreModel(const std::string &Name)
    : CName( Name )
    , m_Path("")
    , m_FileName("")
    , m_CalHardwareModel(0)
    , m_RenderableVertexs(0)
    , m_CalCoreModel(new CalCoreModel(Name))
    , m_NumFaces(0)
    , m_NumVtxs(0)
{
}

CAnimatedCoreModel::~CAnimatedCoreModel()
{
    Destroy();
}

void CAnimatedCoreModel::Destroy()
{
    m_TextureVector.clear();
    m_AnimationsMap.clear();

    CHECKED_DELETE(m_CalCoreModel);
    CHECKED_DELETE(m_CalHardwareModel);
    CHECKED_DELETE(m_RenderableVertexs);
}

bool CAnimatedCoreModel::LoadMesh(const std::string &Filename)
{
    ASSERT(m_CalCoreModel, "Cal Core Model not found");
    std::string &MeshFullPath = m_Path + Filename;
    int err = m_CalCoreModel->loadCoreMesh( MeshFullPath );
    return ( err == -1 ) ? false : true;
}

bool CAnimatedCoreModel::LoadSkeleton(const std::string &Filename)
{
    ASSERT(m_CalCoreModel, "Cal Core Model not found");
    return m_CalCoreModel->loadCoreSkeleton(m_Path + Filename);
}
bool CAnimatedCoreModel::LoadAnimation(const std::string &aName, const std::string &aFilename)
{
    ASSERT(m_CalCoreModel, "Cal Core Model not found");

    bool lLoadedOk( true );

    int lId = m_CalCoreModel->loadCoreAnimation(m_Path + aFilename, aName);

    lLoadedOk = lLoadedOk && bool( lId != -1 );

    if( lLoadedOk )
    {
        if( m_AnimationsMap.find( aName ) != m_AnimationsMap.end() )
        {
            LOG_ERROR_APPLICATION( "Adding twice the animation %s", aName.c_str() );
        }
        else
        {
            m_AnimationsMap[aName] = lId;
        }
    }

    return lLoadedOk;
}

bool CAnimatedCoreModel::LoadVertexBuffer(CGraphicsManager *GM)
{
    m_NumVtxs = 0;
    m_NumFaces = 0;
    for(int i = 0; i < m_CalCoreModel->getCoreMeshCount(); ++i)
    {
        CalCoreMesh *l_CalCoreMesh = m_CalCoreModel->getCoreMesh(i);
        for(int j = 0; j < l_CalCoreMesh->getCoreSubmeshCount(); ++j)
        {
            CalCoreSubmesh *l_CalCoreSubmesh = l_CalCoreMesh->getCoreSubmesh(j);
            if(l_CalCoreSubmesh != NULL)
            {
                m_NumVtxs += l_CalCoreSubmesh->getVertexCount();
                m_NumFaces += l_CalCoreSubmesh->getFaceCount();
            }
        }
    }

    
    CHECKED_DELETE(m_CalHardwareModel);
    m_CalHardwareModel = new CalHardwareModel(m_CalCoreModel);
    unsigned short *l_Idxs = new unsigned short[m_NumFaces * 3];
    CAL3D_HW_VERTEX *l_Vtxs = new CAL3D_HW_VERTEX[m_NumFaces * 3];

    m_CalHardwareModel->setVertexBuffer((char*) l_Vtxs, sizeof(CAL3D_HW_VERTEX));
    m_CalHardwareModel->setWeightBuffer(((char*) l_Vtxs) + 12, sizeof(CAL3D_HW_VERTEX));
    m_CalHardwareModel->setMatrixIndexBuffer(((char*) l_Vtxs) + 28, sizeof(CAL3D_HW_VERTEX));

    m_CalHardwareModel->setNormalBuffer(((char*) l_Vtxs) + 44, sizeof(CAL3D_HW_VERTEX));

    m_CalHardwareModel->setTextureCoordNum(1);
    m_CalHardwareModel->setTextureCoordBuffer(0, ((char*)l_Vtxs) + 92, sizeof(CAL3D_HW_VERTEX));

    m_CalHardwareModel->setIndexBuffer(l_Idxs);
    m_CalHardwareModel->load( 0, 0, MAXBONES);

    ASSERT( m_NumVtxs == m_CalHardwareModel->getTotalVertexCount() ,"Different vtx count" );
    ASSERT( m_NumFaces == m_CalHardwareModel->getTotalFaceCount(), "Different faces count" );

    //En caso de utilizar NormalMap
    CalcTangentsAndBinormals 
    (
        l_Vtxs,
        (unsigned short *) l_Idxs,
        m_NumVtxs,
        m_NumFaces * 3,
        sizeof(CAL3D_HW_VERTEX),
        0,
        44,
        60,
        76,
        92
    );

    CHECKED_DELETE(m_RenderableVertexs);
    m_RenderableVertexs = new CIndexedVertexs<CAL3D_HW_VERTEX, short>(GM, l_Vtxs, l_Idxs, m_NumVtxs, m_NumFaces * 3);
    CHECKED_DELETE_ARRAY( l_Vtxs );
    CHECKED_DELETE_ARRAY( l_Idxs );

    return true;
}

const std::string & CAnimatedCoreModel::GetTextureName( size_t id )
{
    return m_TextureVector[id]->GetFileName();
}

size_t CAnimatedCoreModel::GetNumTextures()
{
    return m_TextureVector.size();
}

bool CAnimatedCoreModel::Load(const std::string &Path)
{
    m_Path = Path;
    m_FileName = m_Path + defaultXML;
    return Load();
}

bool CAnimatedCoreModel::Load()
{
    CXMLTreeNode newFile, node;
    if ( !newFile.LoadAndFindNode( m_FileName.c_str(), "animated_model", node ) )
    {
        LOG_ERROR_APPLICATION( "CAnimatedCoreModel::Load No se puede abrir \"%s\"!", m_FileName.c_str());
        return false;
    }

    // Parse the animation stuff
    for(uint32 i = 0, lCount = node.GetNumChildren(); i < lCount ; ++i)
    {
        const CXMLTreeNode& lCurrentNode = node(i);
        const std::string &TagName = lCurrentNode.GetName();

        if( TagName == "texture" )
        {
          m_TextureVector.push_back( lCurrentNode.GetAttribute<CTexture>("file") );
        }
        else if( TagName == "skeleton" )
        {
            const std::string &skeletonFilename = lCurrentNode.GetAttribute<std::string>("file", "no_file");
            if(!LoadSkeleton(skeletonFilename))
            {
                LOG_ERROR_APPLICATION( "CAnimatedCoreModel::LoadSkeleton No se puede abrir %s!", m_FileName.c_str());
            }
        }
        else if( TagName == "mesh" )
        {
            const std::string &meshFilename = lCurrentNode.GetAttribute<std::string>("file", "no_file");
            if(!LoadMesh(meshFilename))
            {
                LOG_ERROR_APPLICATION( "CAnimatedCoreModel::LoadMesh No se puede abrir %s!", m_FileName.c_str());
            }
        }
        else if( TagName == "animation" )
        {
            const std::string &animationFilename = lCurrentNode.GetAttribute<std::string>("file", "no_file");
            const std::string &name = lCurrentNode.GetAttribute<std::string>("name", "no_name");
            if(!LoadAnimation(name, animationFilename))
            {
                LOG_ERROR_APPLICATION( "CAnimatedCoreModel::LoadAnimation No se puede abrir %s!", m_FileName.c_str());
            }
        }
        else if( TagName == "animation_states")
        {
            LoadAnimationStates(lCurrentNode);
        }
        else if( TagName == "animation_changes")
        {
            LoadAnimationChanges(lCurrentNode);
        }
    }

    return true;
}

int CAnimatedCoreModel::GetAnimationId(const std::string &AnimationName) const
{
    int lAnimationId = INT_MAX;

    TAnimationsIdMap::const_iterator lItFind = m_AnimationsMap.find( AnimationName );
    if( lItFind == m_AnimationsMap.end() )
    {
        //LOG_ERROR_APPLICATION( "Unknown animation %s", AnimationName.c_str() );
    }
    else
    {
        lAnimationId = lItFind->second;
        //LOG_INFO_APPLICATION( "Playing Animation %s", AnimationName.c_str() );
    }

    return lAnimationId;
}

int CAnimatedCoreModel::GetAnimationsCount()
{
    return m_AnimationsMap.size();
}

CalCoreModel *CAnimatedCoreModel::GetCoreModel( )
{
    return m_CalCoreModel;
}

CalHardwareModel* CAnimatedCoreModel::GetCalHardwareModel()
{
    return m_CalHardwareModel;
}

CRenderableVertexs* CAnimatedCoreModel::GetRenderableVertexs()
{
    return m_RenderableVertexs;
}

bool CAnimatedCoreModel::Reload()
{
  // Only reload the animations states and the changes
  m_AnimationStates.clear();
  m_AnimationChanges.clear();
  CXMLTreeNode newFile, node;
  if ( !newFile.LoadAndFindNode( m_FileName.c_str(), "animated_model", node ) )
  {
    LOG_ERROR_APPLICATION( "CAnimatedCoreModel::Load No se puede abrir \"%s\"!", m_FileName.c_str());
    return false;
  }

  // Parse the animation stuff
  for(uint32 i = 0, lCount = node.GetNumChildren(); i < lCount ; ++i)
  {
    const CXMLTreeNode& lCurrentNode = node(i);
    const std::string &TagName = lCurrentNode.GetName();

    if( TagName == "animation_states")
    {
      LoadAnimationStates(lCurrentNode);
    }
    else if( TagName == "animation_changes")
    {
      LoadAnimationChanges(lCurrentNode);
    }
  }

  return true;
}

void CAnimatedCoreModel::ActivateTextures()
{
    TTextureVector::iterator itb = m_TextureVector.begin(), ite = m_TextureVector.end();

    for(size_t i = 0; itb != ite ; ++itb, ++i)
    {
        (*itb)->Activate(i);
    }
}

void CAnimatedCoreModel::ReloadAnimationStates()
{
    CXMLTreeNode l_treeActor;
    if(l_treeActor.LoadFile(m_FileName.c_str()))
    {
        LOG_INFO_APPLICATION("CAnimatedCoreModel::ReloadAnimaionStates Loading \"%s\"", m_FileName.c_str());
        CXMLTreeNode l_xmlAnimationStates = l_treeActor["animation_states"];
        if(l_xmlAnimationStates.Exists())
        {
            LoadAnimationStates(l_xmlAnimationStates);
        }

        CXMLTreeNode l_xmlAnimationChanges = l_treeActor["animation_changes"];
        if(l_xmlAnimationChanges.Exists())
        {
            LoadAnimationChanges(l_xmlAnimationChanges);
        }
    } else 
    {
        LOG_WARNING_APPLICATION("CAnimatedCoreModel::ReloadAnimaionStates No s'ha trobat l'arxiu \"%s\"", m_FileName.c_str());
    }
}

void CAnimatedCoreModel::LoadAnimationStates( const CXMLTreeNode& _xmlAnimationStates)
{
    m_szDefaultAnimationState = _xmlAnimationStates.GetAttribute<std::string>("default", "");
    for(int i = 0, l_iNumChild = _xmlAnimationStates.GetNumChildren(); i < l_iNumChild; ++i)
    {
        const CXMLTreeNode& l_xmlAnimationState = _xmlAnimationStates(i);
        const std::string& lTagName = l_xmlAnimationState.GetName();
        if( lTagName == "animation_state" )
        {
            SAnimationState l_AnimationState;
            l_AnimationState.mName =  l_xmlAnimationState.GetAttribute<std::string>("name", "no_name");
            if( LoadAnimationState(l_xmlAnimationState, l_AnimationState) )
            {
                m_AnimationStates[l_AnimationState.mName] = l_AnimationState;
            }
        }
    }
}

void CAnimatedCoreModel::LoadAnimationChanges( const CXMLTreeNode& _xmlAnimationChanges)
{
    for(int i = 0, l_iNumChild = _xmlAnimationChanges.GetNumChildren(); i < l_iNumChild; ++i)
    {
        CXMLTreeNode l_xmlAnimationChange = _xmlAnimationChanges(i);
        if(strcmp(l_xmlAnimationChange.GetName(), "animation_change") == 0)
        {
            std::string l_szFrom = l_xmlAnimationChange.GetAttribute<std::string>("from", "");
            std::string l_szTo = l_xmlAnimationChange.GetAttribute<std::string>("to", "");
            SAnimationChange l_AnimationChange;

            if(LoadAnimationChange(l_xmlAnimationChange, l_AnimationChange))
            {
                m_AnimationChanges[l_szFrom][l_szTo] = l_AnimationChange;
            }

        }
    }
}

bool CAnimatedCoreModel::LoadAnimationFromState( const CXMLTreeNode &_xmlAnimation, CAnimatedCoreModel::SAnimation &Animation_)
{
    Animation_.mName = _xmlAnimation.GetAttribute<std::string>("name", "no_name");
    Animation_.iId = m_CalCoreModel->getCoreAnimationId(Animation_.mName);

    if(Animation_.iId < 0)
    {
        LOG_WARNING_APPLICATION("CAnimatedCoreModel::LoadAnimationFromState Cicle \"%s\" no existeix", Animation_.mName.c_str());
        return false;
    }

    Animation_.fWeight = _xmlAnimation.GetAttribute<float>("weight", 1.f);

    Animation_.bFromParameter = _xmlAnimation.GetAttribute<bool>("weight_from_parameter", false);
    Animation_.bFromComplementaryParameter = _xmlAnimation.GetAttribute<bool>("weight_from_complementary_parameter", false);


    if(Animation_.bFromParameter && Animation_.bFromComplementaryParameter)
    {
        LOG_WARNING_APPLICATION("CAnimatedCoreModel::LoadAnimationFromState Cicle \"%s\" te \"weight_from_parameter\" i \"weight_from_complementary_parameter\" actius a la vegada", Animation_.mName.c_str());
        return false;
    }
    else if(Animation_.bFromParameter || Animation_.bFromComplementaryParameter)
    {
        Animation_.fFadeOnChange = _xmlAnimation.GetAttribute<float>("fade_on_change", .15f);
    }

    return true;
}

bool CAnimatedCoreModel::LoadActionFromState( const CXMLTreeNode &_xmlAction, CAnimatedCoreModel::SAction &Action_)
{
    assert(strcmp(_xmlAction.GetName(), "action") == 0);

    if(!LoadAnimationFromState(_xmlAction, Action_))
        return false;

    Action_.bBlock   = _xmlAction.GetAttribute<bool>("block", false);
    Action_.bStop    = _xmlAction.GetAttribute<bool>("stop", false);
    Action_.fFadeIn  = _xmlAction.GetAttribute<float>("fade_in", .3f);
    Action_.fFadeOut = _xmlAction.GetAttribute<float>("fade_out", .3f);

    if(Action_.bBlock && Action_.bStop)
    {
        LOG_WARNING_APPLICATION("CAnimatedCoreModel::LoadActionFromState action can not block and stop");
    }
    return true;
}

bool CAnimatedCoreModel::LoadAnimationState( const CXMLTreeNode &_xmlAnimationState, CAnimatedCoreModel::SAnimationState &AnimationState_)
{
    AnimationState_.fDefaultFadeIn  = _xmlAnimationState.GetAttribute<float>("default_fade_in", 0.3f);
    AnimationState_.fDefaultFadeOut = _xmlAnimationState.GetAttribute<float>("default_fade_out", 0.3f);
    for(int i = 0, l_iNumChild = _xmlAnimationState.GetNumChildren(); i < l_iNumChild; ++i)
    {
        const CXMLTreeNode &lCurrentNode = _xmlAnimationState(i);
        const std::string& lTagName      =  lCurrentNode.GetName();
        if( lTagName == std::string( "cycle" ) )
        {
            CAnimatedCoreModel::SCycle l_Cycle;
            if(LoadAnimationFromState(lCurrentNode, l_Cycle))
                AnimationState_.Cycles.insert(l_Cycle);
        }
        else if( lTagName == std::string( "on_enter" ) )
        {
            for(int j = 0, lCount = lCurrentNode.GetNumChildren(); j < lCount; ++j )
            {
                const CXMLTreeNode& l_xmlAction = lCurrentNode(i);
                if(l_xmlAction.GetName() == std::string( "action") )
                {
                    CAnimatedCoreModel::SAction l_Action;
                    if(LoadActionFromState(l_xmlAction, l_Action))
                        AnimationState_.OnEnter.insert(l_Action);
                }
            }   
        }
        else if( lTagName == std::string( "on_exit" ) )
        {
            for(int j = 0, lCount = lCurrentNode.GetNumChildren(); j < lCount; ++j )
            {
                const CXMLTreeNode& l_xmlAction = lCurrentNode(j);
                if(l_xmlAction.GetName() == std::string( "action") )
                {
                    CAnimatedCoreModel::SAction l_Action;
                    if(LoadActionFromState(l_xmlAction, l_Action))
                        AnimationState_.OnExit.insert(l_Action);
                }
            }   
        }
    }
    return true;
}

bool CAnimatedCoreModel::LoadAnimationChange( const CXMLTreeNode &_xmlAnimationChange, CAnimatedCoreModel::SAnimationChange &AnimationChange_)
{
    AnimationChange_.fFade  = _xmlAnimationChange.GetAttribute<float>("fade", 0.3f);
    int l_iNumChild = _xmlAnimationChange.GetNumChildren();
    for(int i = 0; i < l_iNumChild; ++i)
    {
        const CXMLTreeNode& l_xmlAction = _xmlAnimationChange(i);
        if( l_xmlAction.GetName() == std::string( "action" ) )
        {
            CAnimatedCoreModel::SAction l_Action;
            if(LoadActionFromState(l_xmlAction, l_Action))
                AnimationChange_.Actions.insert(l_Action);
        }
    }
    return true;
}