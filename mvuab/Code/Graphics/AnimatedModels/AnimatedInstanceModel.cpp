#include "AnimatedInstanceModel.h"
#include "AnimatedModelsManager.h"
#include "AnimatedCoreModel.h"
#include "cal3d\cal3d.h"
#include "RenderableVertex\VertexTypes.h"

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
    RenderModelBySoftware(RM);
}

void
CAnimatedInstanceModel::RenderModelBySoftware(CGraphicsManager *RM)
{
  // get the renderer of the model
  CalRenderer *l_pCalRenderer = m_CalModel->getRenderer();

  // begin the rendering loop
  if(!l_pCalRenderer->beginRendering()) return;

  m_VBCursor=0;
  m_IBCursor=0;

  DWORD dwVBLockFlags=D3DLOCK_NOOVERWRITE;
  DWORD dwIBLockFlags=D3DLOCK_NOOVERWRITE;

  LPDIRECT3DDEVICE9 l_pD3DDevice = RM->GetDevice();

  l_pD3DDevice->SetStreamSource( 0, m_pVB, 0,  sizeof(TNORMAL_T1_VERTEX) );
  l_pD3DDevice->SetFVF(TNORMAL_T1_VERTEX::GetFVF());
  l_pD3DDevice->SetIndices(m_pIB);

  // get the number of meshes
  uint32 meshCount = l_pCalRenderer->getMeshCount();

  // render all meshes of the model
  for( uint32 meshId = 0; meshId < meshCount; ++meshId)
  {
    // get the number of submeshes
    uint32 submeshCount = l_pCalRenderer->getSubmeshCount(meshId);

    // render all submeshes of the mesh
    for(uint32 submeshId = 0; submeshId < submeshCount; ++submeshId)
    {
      // select mesh and submesh for further data access
      if(l_pCalRenderer->selectMeshSubmesh(meshId, submeshId))
      {
          /*if(m_VBCursor+ l_pCalRenderer->getVertexCount()>=30000)
          {
                m_VBCursor=0;
              dwVBLockFlags=D3DLOCK_DISCARD;
          }
          
          if(m_IBCursor + pCalRenderer->getFaceCount()>=50000)
          {
                m_IBCursor=0;
              dwIBLockFlags=D3DLOCK_DISCARD;

          }*/
          

          // Get vertexbuffer from the model
          TNORMAL_T1_VERTEX *pVertices;

          m_pVB->Lock(m_VBCursor*sizeof(TNORMAL_T1_VERTEX), l_pCalRenderer->getVertexCount()*sizeof(TNORMAL_T1_VERTEX), (void**)&pVertices, dwVBLockFlags);

          int vertexCount = l_pCalRenderer->getVerticesNormalsAndTexCoords(&pVertices->x);
          m_pVB->Unlock();
          
          CalIndex *meshFaces;

          int faceCount;

          m_pIB->Lock(m_IBCursor* 3*sizeof(CalIndex), l_pCalRenderer->getFaceCount()*3* sizeof(CalIndex), (void**)&meshFaces,dwIBLockFlags);

          faceCount = l_pCalRenderer->getFaces(meshFaces);
          m_pIB->Unlock();


          l_pD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
          //l_pD3DDevice->SetTexture(0,(LPDIRECT3DTEXTURE9)l_pCalRenderer->getMapUserData(0));
          
          l_pD3DDevice->DrawIndexedPrimitive(
              D3DPT_TRIANGLELIST,
              m_VBCursor,
              0,
              vertexCount,
              m_IBCursor*3,
              faceCount
            );

          m_VBCursor+=vertexCount;
          m_IBCursor+=faceCount;

          dwIBLockFlags=D3DLOCK_NOOVERWRITE;
          dwVBLockFlags=D3DLOCK_NOOVERWRITE;

      }
    }
  }


  // end the rendering
  l_pCalRenderer->endRendering();
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