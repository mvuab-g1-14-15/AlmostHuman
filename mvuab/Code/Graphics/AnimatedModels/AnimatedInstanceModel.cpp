#include "AnimatedInstanceModel.h"
#include "AnimatedModelsManager.h"
#include "AnimatedCoreModel.h"
#include "cal3d\cal3d.h"

CAnimatedInstanceModel::CAnimatedInstanceModel() : 
    m_CalModel(0),
    m_AnimatedCoreModel(0),
    m_BlendTime(0.3f),
    m_LodLevel(1.0f),
    m_CurrentAnimationId(0),
    m_NumVtxs(0),
    m_NumFaces(0)
{
}

CAnimatedInstanceModel::~CAnimatedInstanceModel()
{
  Destroy();
}

void
CAnimatedInstanceModel::Render(CGraphicsManager *RM)
{
  // get the renderer of the model
  CalRenderer *pCalRenderer;
  pCalRenderer = m_CalModel->getRenderer();

  // begin the rendering loop
  if(!pCalRenderer->beginRendering())
    return;

  // finish rendering loop
  pCalRenderer->endRendering();
}

void
CAnimatedInstanceModel::RenderModelBySoftware(CGraphicsManager *RM)
{

}

void
CAnimatedInstanceModel::Update(float32 deltaTime)
{
  m_CalModel->update(deltaTime);
}

void
CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *AnimatedCoreModel, CGraphicsManager *RM)
{
  // Create the calcoremodel
  m_AnimatedCoreModel = AnimatedCoreModel;
  CalCoreModel *l_CalCoreModel = m_AnimatedCoreModel->GetCoreModel();
  m_CalModel = new CalModel( l_CalCoreModel );

  // attach all meshes to the model
  uint16 l_CoreMeshCount = l_CalCoreModel->getCoreMeshCount();
  for( uint16 meshId = 0; meshId < l_CoreMeshCount; ++meshId )
  {
    m_CalModel->attachMesh( meshId );
  }
  
  // set the material set of the whole model
  m_CalModel->setMaterialSet(0);

  // set initial animation state
  float32 l_Weight(1.0f), l_DelayIn(0.0f);
  if(l_CalCoreModel->getCoreAnimationCount() > 1)
  {
    l_Weight = 0.0f;
    l_DelayIn = m_BlendTime;
  }

  BlendCycle(m_CurrentAnimationId, l_Weight, l_DelayIn);
}

void
CAnimatedInstanceModel::Destroy()
{
  CHECKED_DELETE(m_CalModel);
  CHECKED_RELEASE(m_pVB);
  CHECKED_RELEASE(m_pIB);
}

void
CAnimatedInstanceModel::ExecuteAction(uint32 Id, float32 DelayIn, float32 DelayOut, float32 WeightTarget, bool AutoLock)
{
  m_CalModel->getMixer()->executeAction(Id,DelayIn,DelayOut,WeightTarget,AutoLock);
}

void
CAnimatedInstanceModel::BlendCycle(uint32 Id, float32 Weight, float32 DelayIn)
{
  m_CalModel->getMixer()->blendCycle(Id, Weight, DelayIn);
}

void
CAnimatedInstanceModel::ClearCycle(uint32 Id, float32 DelayOut)
{
  m_CalModel->getMixer()->clearCycle(Id,DelayOut);
}

bool
CAnimatedInstanceModel::IsCycleAnimationActive(uint32 Id) const
{
  CalCoreAnimation * l_pAnimation = m_AnimatedCoreModel->GetCoreModel()->getCoreAnimation(Id);
  const std::list<CalAnimationCycle *> &l_AnimList = m_CalModel->getMixer()->getAnimationCycle();
  std::list<CalAnimationCycle *>::const_iterator itb = l_AnimList.begin(), ite = l_AnimList.end();

  for(; itb != ite;++itb)
  {
    if((*itb)->getCoreAnimation()==l_pAnimation)
      return true;
  }

  return false;
}

bool
CAnimatedInstanceModel::IsActionAnimationActive(uint32 Id) const
{
  const std::vector<CalAnimation *> &l_AnimVect = m_CalModel->getMixer()->getAnimationVector();
  return ( l_AnimVect[Id] != NULL );
}

bool
CAnimatedInstanceModel::LoadVertexBuffer(CGraphicsManager *RM)
{
  return true;
}

void
CAnimatedInstanceModel::LoadTextures()
{

}
