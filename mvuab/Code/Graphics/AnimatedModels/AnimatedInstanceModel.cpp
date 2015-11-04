#include "AnimatedInstanceModel.h"
#include "AnimatedModelsManager.h"
#include "AnimatedCoreModel.h"

#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableVertex\VertexTypes.h"
#include "Texture\TextureManager.h"

#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"

#include "RenderableVertex\RenderableVertex.h"

#include "Timer\Timer.h"
#include "EngineManagers.h"

#include "ActionManager.h"
#include "Utils\Defines.h"

#include "XML\XMLTreeNode.h"

#include "Lights\LightProbe.h"
#include "RenderableObject\Scene.h"

#include "Timer\CountDownTimerManager.h"
#include <cal3d\corebone.h>
#include "Bone\PhysxBone.h"

#include <boost\foreach.hpp>

#define MAXBONES 29

CAnimatedInstanceModel::CAnimatedInstanceModel( const std::string& Name, const std::string& CoreName )
  : CRenderableObject()
  , m_CalModel( 0 )
  , m_AnimatedCoreModel( AnimatedMInstance->GetCore( CoreName ) )
  , m_BlendTime( 0.3f )
  , m_CurrentAnimationId( 0 )
  , m_NumVtxs( 0 )
  , m_NumFaces( 0 )
  , m_pIB( 0 )
  , m_pVB( 0 )
  , m_IBCursor( 0 )
  , m_VBCursor( 0 )
  , m_Velocity( 1.0 )
  , m_ChangeAnimation( 0 )
  , m_RenderableObjectTechnique( 0 )
  , m_PreviousPosition( Math::Vect3f(0.0f,0.0f,0.0f))
  , m_fAnimationParameter( 0.0f )
  , m_szAnimationState( "" )
{
  ZeroMemory( &m_LPMatrixInitial, gLightProbeSize * sizeof( float ));
  ZeroMemory( &m_LPMatrixTarget, gLightProbeSize * sizeof( float ));
  SetName( Name );
  CRenderableObjectTechniqueManager* lROT = ROTMInstance;
  const std::string& l_TechniqueName = lROT->GetRenderableObjectTechniqueNameByVertexType( CAL3D_HW_VERTEX::GetVertexType() );
  m_RenderableObjectTechnique = lROT->GetResource( l_TechniqueName );
  Initialize();
}

CAnimatedInstanceModel::CAnimatedInstanceModel( const CXMLTreeNode& atts )
  : CRenderableObject( atts )
  , m_CalModel( 0 )
  , m_AnimatedCoreModel( AnimatedMInstance->GetCore( atts.GetAttribute<std::string>( "core", "" ) ) )
  , m_BlendTime( 0.3f )
  , m_CurrentAnimationId( 0 )
  , m_NumVtxs( 0 )
  , m_NumFaces( 0 )
  , m_pIB( 0 )
  , m_pVB( 0 )
  , m_IBCursor( 0 )
  , m_VBCursor( 0 )
  , m_Velocity( 1.0 )
  , m_ChangeAnimation( 0 )
  , m_RenderableObjectTechnique( 0 )
  , m_PreviousPosition( Math::Vect3f(0.0f,0.0f,0.0f))
  , m_fAnimationParameter( 0.0f )
  , m_szAnimationState( "" )
{
  ZeroMemory( &m_LPMatrixInitial, gLightProbeSize * sizeof( float ));
  ZeroMemory( &m_LPMatrixTarget, gLightProbeSize * sizeof( float ));

  CRenderableObjectTechniqueManager* lROT = ROTMInstance;
  const std::string& l_TechniqueName = lROT->GetRenderableObjectTechniqueNameByVertexType(
                                         CAL3D_HW_VERTEX::GetVertexType() );
  m_RenderableObjectTechnique = lROT->GetResource( l_TechniqueName );
  Initialize();
}
CAnimatedInstanceModel::~CAnimatedInstanceModel()
{
  Destroy();
}

void CAnimatedInstanceModel::Render()
{
  CGraphicsManager* lGPMan = GraphicsInstance;
  lGPMan->SetTransform( GetTransform() );
  RenderModelByHardware();
  lGPMan->SetTransform( Math::Mat44f() );
}

void CAnimatedInstanceModel::RenderModelByHardware()
{
  if ( !m_RenderableObjectTechnique )
  {
    LOG_INFO_APPLICATION("Null effect technique!");
    return;
  }

  // Get the shader of the current pool of effects
  CEffectTechnique* lEffectTechnique = m_RenderableObjectTechnique->GetEffectTechnique();

  ASSERT( lEffectTechnique, "Null  Effect technique to render the animated instance model %s", GetName().c_str() );

  CEffect* lEffect = lEffectTechnique->GetEffect();

  ASSERT( lEffect, "Null  Effect to load the animated render model %s", GetName().c_str() );

  LPD3DXEFFECT lDXEffect = lEffect->GetEffect();

  ASSERT( lDXEffect, "Null  Effect to load the animated render model %s", GetName().c_str() );

  lEffectTechnique->BeginRender();
  CalHardwareModel* l_pCalHardwareModel = m_AnimatedCoreModel->GetCalHardwareModel();
  D3DXMATRIX transformation[MAXBONES];

  for( int hardwareMeshId = 0, lCountHardwareMesh = l_pCalHardwareModel->getHardwareMeshCount();
      hardwareMeshId < lCountHardwareMesh;
      ++hardwareMeshId
  )
  {
    l_pCalHardwareModel->selectHardwareMesh( hardwareMeshId );

    for ( int boneId = 0; boneId < l_pCalHardwareModel->getBoneCount(); ++boneId )
    {
      D3DXMatrixRotationQuaternion( &transformation[boneId],
                                    ( CONST D3DXQUATERNION* )&l_pCalHardwareModel->getRotationBoneSpace( boneId,
                                        m_CalModel->getSkeleton() ) );
      CalVector translationBoneSpace = l_pCalHardwareModel->getTranslationBoneSpace( boneId,
                                       m_CalModel->getSkeleton() );
      transformation[boneId]._14 = translationBoneSpace.x;
      transformation[boneId]._24 = translationBoneSpace.y;
      transformation[boneId]._34 = translationBoneSpace.z;
    }

    float l_Matrix[MAXBONES * 3 * 4];
    ZeroMemory( &l_Matrix, sizeof( float ) * MAXBONES * 3 * 4 );

    for ( int i = 0; i < l_pCalHardwareModel->getBoneCount(); ++i )
    {
      memcpy( &l_Matrix[i * 3 * 4], &transformation[i], sizeof( float ) * 3 * 4 );
    }

    lDXEffect->SetFloatArray( lEffect->GetBonesParameter(), ( float* ) l_Matrix, l_pCalHardwareModel->getBoneCount() * 3 * 4 );
    
    CalculateNewLightProbeMatrix();
    float l_LPMatrix[gLightProbeSize*2 +1];
    for (unsigned int i=0; i<gLightProbeSize; ++i)
      l_LPMatrix[i] = m_LPMatrixInitial[i];
    for (unsigned int i=0; i<gLightProbeSize; ++i)
      l_LPMatrix[i+gLightProbeSize] = m_LPMatrixTarget[i];
    l_LPMatrix[gLightProbeSize*2] = CountDownTimerInstance->GetElapsedTimeInPercent( GetName() + "LightProbeTimer" );
    
    lDXEffect->SetFloatArray( lEffect->GetLightProbesParameter(), ( float* ) l_LPMatrix, gLightProbeSize*2 + 1 );

    m_Textures[0]->Activate( 0 );

	// Activate light probe texture
	//TextureMInstance->GetTexture("Data/room2/LightProbeLightingMap.tga")->Save("lightprobe");
    const std::string lRoomName = GetRoomName();
    ASSERT(lRoomName != "",  "The object %s has no room", GetName().c_str() );
    if (lRoomName != "")
	    TextureMInstance->GetTexture("Data/"+GetRoomName()+"/LightProbeLightingMap.tga")->Activate(6); //Hardcoded to test

    //m_NormalTextureList[0]->Activate(1);
    m_AnimatedCoreModel->GetRenderableVertexs()->Render
    (
      GraphicsInstance,
      lEffectTechnique,
      l_pCalHardwareModel->getBaseVertexIndex(),
      0,
      l_pCalHardwareModel->getVertexCount(),
      l_pCalHardwareModel->getStartIndex(),
      l_pCalHardwareModel->getFaceCount()
    );
  }
}

void CAnimatedInstanceModel::RenderModelBySoftware()
{
  CGraphicsManager* RM = GraphicsInstance;
  CalRenderer* l_pCalRenderer = m_CalModel->getRenderer();

  if ( !l_pCalRenderer->beginRendering() )
    return;

  m_VBCursor = 0;
  m_IBCursor = 0;

  DWORD dwVBLockFlags = D3DLOCK_NOOVERWRITE;
  DWORD dwIBLockFlags = D3DLOCK_NOOVERWRITE;
  LPDIRECT3DDEVICE9 l_pD3DDevice = RM->GetDevice();

  l_pD3DDevice->SetStreamSource( 0, m_pVB, 0, sizeof( TNORMAL_T1_VERTEX ) );
  l_pD3DDevice->SetFVF( TNORMAL_T1_VERTEX::GetFVF() );
  l_pD3DDevice->SetIndices( m_pIB );

  uint32 meshCount = l_pCalRenderer->getMeshCount();

  for ( uint32 meshId = 0; meshId < meshCount; ++meshId )
  {
    uint32 submeshCount = l_pCalRenderer->getSubmeshCount( meshId );

    for ( uint32 submeshId = 0; submeshId < submeshCount; ++submeshId )
    {
      if ( false == l_pCalRenderer->selectMeshSubmesh( meshId, submeshId ) )
        continue;

      if ( m_VBCursor + l_pCalRenderer->getVertexCount() >= 30000 )
      {
        m_VBCursor = 0;
        dwVBLockFlags = D3DLOCK_DISCARD;
      }

      if ( m_IBCursor + l_pCalRenderer->getFaceCount() >= 50000 )
      {
        m_IBCursor = 0;
        dwIBLockFlags = D3DLOCK_DISCARD;
      }

      TNORMAL_T1_VERTEX* pVertices = 0;
      CalIndex* meshFaces = 0;
      int faceCount = 0;

      m_pVB->Lock( m_VBCursor * sizeof( TNORMAL_T1_VERTEX ), l_pCalRenderer->getVertexCount()*sizeof( TNORMAL_T1_VERTEX ),
                   ( void** ) &pVertices, dwVBLockFlags );
      int vertexCount = l_pCalRenderer->getVerticesNormalsAndTexCoords( &pVertices->x );
      m_pVB->Unlock();


      m_pIB->Lock( m_IBCursor * 3 * sizeof( CalIndex ), l_pCalRenderer->getFaceCount() * 3 * sizeof( CalIndex ),
                   ( void** ) &meshFaces, dwIBLockFlags );
      faceCount = l_pCalRenderer->getFaces( meshFaces );
      m_pIB->Unlock();

      l_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

      m_Textures[meshId]->Activate( 0 );
      m_AnimatedCoreModel->ActivateTextures();
      l_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, m_VBCursor, 0, vertexCount, m_IBCursor * 3, faceCount );

      m_VBCursor += vertexCount;
      m_IBCursor += faceCount;

      dwIBLockFlags = D3DLOCK_NOOVERWRITE;
      dwVBLockFlags = D3DLOCK_NOOVERWRITE;
    }
  }

  // end the rendering
  l_pCalRenderer->endRendering();
}

void CAnimatedInstanceModel::Initialize()
{
  ASSERT( m_AnimatedCoreModel, "Invalid CalCoreModel" );

  // Create the calcoremodel
  CalCoreModel* l_CalCoreModel = m_AnimatedCoreModel->GetCoreModel();
  m_CalModel = new CalModel( l_CalCoreModel );

  // attach all meshes to the model
  uint16 l_CoreMeshCount = l_CalCoreModel->getCoreMeshCount();

  for ( uint16 meshId = 0; meshId < l_CoreMeshCount; ++meshId )
    m_CalModel->attachMesh( meshId );

  // set the material set of the whole model
  //m_CalModel->setMaterialSet(0);
  // set initial animation state
  /*  float32 l_Weight(1.0f), l_DelayIn(0.0f);
      if(l_CalCoreModel->getCoreAnimationCount() > 1)
      {
      l_Weight = 0.0f;
      l_DelayIn = m_BlendTime;
      }*/

  BlendCycle( 0, 1.0f, 0.0f );
  m_CalModel->update( 0.0f );

  LoadTextures();

  m_PreviousPosition = GetPosition();

  for (unsigned int i = 0; i < gLightProbeSize; ++i)
  {
	  m_LPMatrixInitial[i] = 0.0f;
	  m_LPMatrixTarget[i] = 0.0f;
  }

  CountDownTimerInstance->AddTimer( GetName() + "LightProbeTimer", 1.0f);

  m_szAnimationState = m_AnimatedCoreModel->m_szDefaultAnimationState;
  if(m_szAnimationState != "")
  {
      std::map<std::string, SAnimationState>::iterator l_it = m_AnimatedCoreModel->m_AnimationStates.find(m_szAnimationState);
      if(l_it != m_AnimatedCoreModel->m_AnimationStates.end())
      {
          BOOST_FOREACH(const SCycle& cycle, l_it->second.Cycles)
          {
              float l_fWeight = (cycle.bFromParameter)? m_fAnimationParameter : ((cycle.bFromComplementaryParameter)? 1 - m_fAnimationParameter : 1);
              l_fWeight *= cycle.fWeight;
              m_CalModel->getMixer()->blendCycle(cycle.iId,l_fWeight,0.f);
          }

      } else {
          LOG_WARNING_APPLICATION("CAnimatedInstanceModel::Initialize Invalid default animation state %s", m_szAnimationState.c_str());
          m_szAnimationState = "";
      }
  }
}

void CAnimatedInstanceModel::Destroy()
{
  CHECKED_DELETE( m_CalModel );
  CHECKED_RELEASE( m_pVB );
  CHECKED_RELEASE( m_pIB );
}

void CAnimatedInstanceModel::Update()
{
  BROFILER_CATEGORY( "CAnimatedInstanceModel::Update()", Profiler::Color::Orchid )
  m_CalModel->update( constFrameTime * m_Velocity);
}

void CAnimatedInstanceModel::ChangeAnimation( const std::string& AnimationName, float32 DelayIn, float32 DelayOut )
{
  uint32 l_Id = m_AnimatedCoreModel->GetAnimationId( AnimationName );

  if ( l_Id != m_CurrentAnimationId )
  {
    ClearCycle( m_CurrentAnimationId, DelayOut );
    BlendCycle( l_Id, 1.0f, DelayIn );

    m_CurrentAnimationId = l_Id;
  }
}

void CAnimatedInstanceModel::ChangeAnimationAction( const std::string& AnimationName, float32 DelayIn, float32 DelayOut )
{
  uint32 l_Id = m_AnimatedCoreModel->GetAnimationId( AnimationName );

  if ( l_Id != m_CurrentAnimationId && l_Id != INT_MAX )
  {
    ClearCycle( m_CurrentAnimationId, DelayOut );
    ExecuteAction( l_Id, 1.0f, DelayIn );

    m_CurrentAnimationId = l_Id;
  }
}

void CAnimatedInstanceModel::ExecuteAction( uint32 Id, float32 DelayIn, float32 DelayOut, float32 WeightTarget,
    bool AutoLock )
{
  m_CalModel->getMixer()->removeAction( Id );
  m_CalModel->getMixer()->executeAction( Id, DelayIn, DelayOut, WeightTarget, AutoLock );
}

void CAnimatedInstanceModel::RemoveAction( uint32 Id )
{
  m_CalModel->getMixer()->removeAction( Id );
}

void CAnimatedInstanceModel::BlendCycle( uint32 Id, float32 Weight, float32 DelayIn )
{
  m_CalModel->getMixer()->blendCycle( Id, Weight, DelayIn );
}

void CAnimatedInstanceModel::ClearCycle( uint32 Id, float32 DelayOut )
{
  m_CalModel->getMixer()->clearCycle( Id, DelayOut );
}

bool CAnimatedInstanceModel::IsCycleAnimationActive( uint32 Id ) const
{
  CalCoreAnimation* l_pAnimation = m_AnimatedCoreModel->GetCoreModel()->getCoreAnimation( Id );
  const std::list<CalAnimationCycle*>& l_AnimList = m_CalModel->getMixer()->getAnimationCycle();
  std::list<CalAnimationCycle*>::const_iterator itb = l_AnimList.begin(), ite = l_AnimList.end();

  for ( ; itb != ite; ++itb )
  {
    if ( ( *itb )->getCoreAnimation() == l_pAnimation )
      return true;
  }

  return false;
}

bool CAnimatedInstanceModel::IsActionAnimationActive( uint32 Id ) const
{
  const std::vector<CalAnimation*>& l_AnimVect = m_CalModel->getMixer()->getAnimationVector();
  return l_AnimVect[Id] != NULL;
}

void CAnimatedInstanceModel::LoadTextures()
{
  for ( size_t i = 0; i < m_AnimatedCoreModel->GetNumTextures(); ++i )
  {
    CTexture* l_Texture = TextureMInstance->GetTexture( m_AnimatedCoreModel->GetTextureName( i ) );
    m_Textures.push_back( l_Texture );
  }

  if( m_Textures.empty() )
  {
	  m_Textures.push_back( TextureMInstance->GetTexture("Data/textures/Dummy.png") );
  }
}

void CAnimatedInstanceModel::ExecuteAction( const std::string& AnimationName, float32 DelayIn, float32 DelayOut,
    float32 WeightTarget, bool AutoLock )
{
  uint32 l_Id = m_AnimatedCoreModel->GetAnimationId( AnimationName );
  ExecuteAction( l_Id, DelayIn, DelayOut, WeightTarget, AutoLock );
}

void CAnimatedInstanceModel::ExecuteActionLUA( const std::string& AnimationName, float32 DelayIn, float32 DelayOut )
{
  ExecuteAction(AnimationName, DelayIn, DelayOut);
}

void CAnimatedInstanceModel::RemoveAction( const std::string& AnimationName )
{
  uint32 l_Id = m_AnimatedCoreModel->GetAnimationId( AnimationName );
  RemoveAction( l_Id );
}

void CAnimatedInstanceModel::BlendCycle( const std::string& AnimationName, float32 Weight, float32 DelayIn )
{
  uint32 l_Id = m_AnimatedCoreModel->GetAnimationId( AnimationName );
  BlendCycle( l_Id, Weight, DelayIn );
}

void CAnimatedInstanceModel::ClearCycle( const std::string& AnimationName, float32 DelayOut )
{
  uint32 l_Id = m_AnimatedCoreModel->GetAnimationId( AnimationName );
  ClearCycle( l_Id, DelayOut );
}

bool CAnimatedInstanceModel::IsCycleAnimationActive( const std::string& AnimationName ) const
{
  uint32 l_Id = m_AnimatedCoreModel->GetAnimationId( AnimationName );
  return IsCycleAnimationActive( l_Id );
}

bool CAnimatedInstanceModel::IsActionAnimationActive( const std::string& AnimationName ) const
{
  uint32 l_Id = m_AnimatedCoreModel->GetAnimationId( AnimationName );
  return IsActionAnimationActive( l_Id );
}

void CAnimatedInstanceModel::CalculateNewLightProbeMatrix()
{
	if ( m_PreviousPosition != GetPosition() )
	{
		// Calculate the ambient light with the light probe
		std::vector<CLightProbe*> lLightProbes = SceneInstance->GetClosedLightProbes(GetRoomName(), GetPosition());
	
		float l_LPMatrix[gLightProbeSize];

		unsigned int lCounter = 0;
		CLightProbeVertex *lVect;
		for (unsigned int i = 0; i < lLightProbes.size(); ++i)
		{
			Math::Vect3f lPos( lLightProbes[i]->GetPosition() );
			l_LPMatrix[lCounter++] = lPos.Distance( m_Position ); //factor. tiene que ser dependiente de la distancia por ahora se reparte a todos iguales
			lVect = lLightProbes[i]->GetVertex("x");
			l_LPMatrix[lCounter++] = lVect->GetUV().x;
			l_LPMatrix[lCounter++] = lVect->GetUV().y;
			lVect = lLightProbes[i]->GetVertex("y");
			l_LPMatrix[lCounter++] = lVect->GetUV().x;
			l_LPMatrix[lCounter++] = lVect->GetUV().y;
			lVect = lLightProbes[i]->GetVertex("z");
			l_LPMatrix[lCounter++] = lVect->GetUV().x;
			l_LPMatrix[lCounter++] = lVect->GetUV().y;
			lVect = lLightProbes[i]->GetVertex("-x");
			l_LPMatrix[lCounter++] = lVect->GetUV().x;
			l_LPMatrix[lCounter++] = lVect->GetUV().y;
			lVect = lLightProbes[i]->GetVertex("-y");
			l_LPMatrix[lCounter++] = lVect->GetUV().x;
			l_LPMatrix[lCounter++] = lVect->GetUV().y;
			lVect = lLightProbes[i]->GetVertex("-z");
			l_LPMatrix[lCounter++] = lVect->GetUV().x;
			l_LPMatrix[lCounter++] = lVect->GetUV().y;
		}

		float lTotal( l_LPMatrix[0] + l_LPMatrix[13] + l_LPMatrix[26] + l_LPMatrix[39] );
		l_LPMatrix[0] /= lTotal;
		l_LPMatrix[13] /= lTotal;
		l_LPMatrix[26] /= lTotal;
		l_LPMatrix[39] /= lTotal;

		for (unsigned int i = 0; i < gLightProbeSize; ++i)
			m_LPMatrixTarget[i] = l_LPMatrix[i];
	}
	
	float lPercentage = CountDownTimerInstance->GetElapsedTimeInPercent( GetName() + "LightProbeTimer" );
	if (lPercentage >= 1.0f)
	{
		CountDownTimerInstance->Reset( GetName() + "LightProbeTimer" );
		for (unsigned int i = 0; i < gLightProbeSize; ++i)
			m_LPMatrixInitial[i] = m_LPMatrixTarget[i];
	}

	m_PreviousPosition = GetPosition();
}

Math::Vect3f CAnimatedInstanceModel::GetBonePosition( const std::string& aBoneName )
{
  Math::Vect3f lBonePosition;
  CalSkeleton* l_pSkeleton = m_CalModel->getSkeleton();

  if( l_pSkeleton )
  {

//      const std::vector< CalCoreBone*>& mBones = l_pSkeleton->getCoreSkeleton()->getVectorCoreBone();
//      for( uint32 i = 0; i < mBones.size(); ++i )
//      {
//        LOG_INFO_APPLICATION( "Bone %s ", mBones[i]->getName().c_str() );
//      }

    int l_iBoneId = l_pSkeleton->getCoreSkeleton()->getCoreBoneId(aBoneName);

    if( l_iBoneId > 0 )
    {
      CalBone* l_pBone = l_pSkeleton->getBone(l_iBoneId);

      ASSERT( l_pBone, "Null bone %s", aBoneName.c_str() );

      //rotacio i translacio del bone (absoluta)
      CalVector l_vTranslation = l_pBone->getTranslationAbsolute();
      CalQuaternion l_RotationQuaternion = l_pBone->getRotationAbsolute();

      //passem el quaternion a left handed
      l_RotationQuaternion.x = -l_RotationQuaternion.x;
      l_RotationQuaternion.y = -l_RotationQuaternion.y;
      l_RotationQuaternion.z = -l_RotationQuaternion.z;

      //creem la matriu de transformacio Cal3d (absolute) -> Math::Mat44f
      CalMatrix l_RotationMatrix(l_RotationQuaternion);

      Math::Mat33f l_Rotation = Math::Mat33f( l_RotationMatrix.dxdx   , l_RotationMatrix.dydx  , l_RotationMatrix.dzdx,
        l_RotationMatrix.dxdy   , l_RotationMatrix.dydy  , l_RotationMatrix.dzdy,
        l_RotationMatrix.dxdz   , l_RotationMatrix.dydz  , l_RotationMatrix.dzdz).Get33RotationNormalized();

      float l_fAngleX = Math::pi32 - l_Rotation.GetAngleX();
      float l_fAngleY = Math::pi32 - l_Rotation.GetAngleY();
      float l_fAngleZ = Math::pi32 - l_Rotation.GetAngleZ();

      Math::Mat44f l_Transform;
      l_Transform.SetIdentity();
      l_Transform.RotByAnglesYXZ(l_fAngleY, l_fAngleX, l_fAngleZ);
      l_Transform.Translate(Math::Vect3f(-l_vTranslation.x, l_vTranslation.y, l_vTranslation.z));

      Math::Mat44f l_vTotal = GetTransform()*l_Transform;
      lBonePosition = l_vTotal.GetPos();
    }
  }

  return lBonePosition;
}

float CAnimatedInstanceModel::ExecuteChange(const SAnimationChange& _AnimationChange)
{
    BOOST_FOREACH(const SAction& l_Action, _AnimationChange.Actions)
    {
        ExecuteAction(l_Action);
    }

    return _AnimationChange.fFade;
}

void  CAnimatedInstanceModel::ExecuteAction(const SAction& _Action)
{
    CalMixer *l_pMixer = m_CalModel->getMixer();
    if(_Action.bStop)
    {
        l_pMixer->removeAction(_Action.iId);
    }
    else
    {
        float l_fWeight;
        if(_Action.bFromParameter)
        {
            l_fWeight = m_fAnimationParameter * _Action.fWeight;
        } else if(_Action.bFromComplementaryParameter)
        {
            l_fWeight = (1.f - m_fAnimationParameter) * _Action.fWeight;
        } else
        {
            l_fWeight = _Action.fWeight;
        }

        float l_fBlendIn  = (_Action.fFadeIn  < 0)? 0 : _Action.fFadeIn;
        float l_fBlendOut = (_Action.fFadeOut < 0)? 0 : _Action.fFadeOut;

        l_pMixer->executeAction(_Action.iId, l_fBlendIn, l_fBlendOut, l_fWeight, _Action.bBlock);
    }
}

void  CAnimatedInstanceModel::BlendCycle(const SCycle& _Cycle, float _fBlendTime)
{
    float l_fWeight;
    if(_Cycle.bFromParameter)
    {
        l_fWeight = m_fAnimationParameter * _Cycle.fWeight;
    } else if(_Cycle.bFromComplementaryParameter)
    {
        l_fWeight = (1.f - m_fAnimationParameter) * _Cycle.fWeight;
    } else
    {
        l_fWeight = _Cycle.fWeight;
    }

    _fBlendTime = (_fBlendTime < 0)? 0 : _fBlendTime;

    m_CalModel->getMixer()->blendCycle(_Cycle.iId, l_fWeight, _fBlendTime);
}

void  CAnimatedInstanceModel::ClearCycle(const SCycle& _Cycle, float _fBlendTime)
{
    m_CalModel->getMixer()->clearCycle(_Cycle.iId, _fBlendTime);
}

void CAnimatedInstanceModel::SetAnimationState(const std::string& _szAnimationState)
{
    if(_szAnimationState == m_szAnimationState)
    {
        return;
    }

    std::map<std::string,SAnimationState>::const_iterator l_itNextAnimationState = m_AnimatedCoreModel->m_AnimationStates.find(_szAnimationState);

    if(l_itNextAnimationState == m_AnimatedCoreModel->m_AnimationStates.end())
    {
        LOG_WARNING_APPLICATION("CAnimatedInstanceModel::SetAnimationState intentant canviar a un estat inexistent \"%s\"", _szAnimationState.c_str());
        return;
    }

    const SAnimationState& l_NextAnimationState = l_itNextAnimationState->second;

    float l_fFade = l_NextAnimationState.fDefaultFadeIn;

    const std::set<SCycle>& l_CyclesToActivate = l_NextAnimationState.Cycles;

    if(m_szAnimationState != "")
    {
        SAnimationState &l_PrevAnimationState = m_AnimatedCoreModel->m_AnimationStates.find(m_szAnimationState)->second;

        BOOST_FOREACH(const SAction& l_Action, l_PrevAnimationState.OnExit)
        {
            ExecuteAction(l_Action);
        }

        const std::map<std::string, SAnimationChange>& l_ChangesFromPrev = m_AnimatedCoreModel->m_AnimationChanges[m_szAnimationState];
        std::map<std::string, SAnimationChange>::const_iterator l_it = l_ChangesFromPrev.find(_szAnimationState);

        if(l_it != l_ChangesFromPrev.end())
        {
            l_fFade = ExecuteChange(l_it->second);
        }
        else
        {
            l_fFade += l_PrevAnimationState.fDefaultFadeOut;
            l_fFade *= .5f;
        }

        const set<SCycle>& l_PrevCycles = l_PrevAnimationState.Cycles;

        BOOST_FOREACH(const SCycle& l_Cycle, l_PrevAnimationState.Cycles)
        {
            if(l_CyclesToActivate.find(l_Cycle) == l_CyclesToActivate.end())
            {
                ClearCycle(l_Cycle, l_fFade);
            }
        }
    }


    BOOST_FOREACH(const SAction& l_Action, l_NextAnimationState.OnEnter)
    {
        ExecuteAction(l_Action);
    }

    BOOST_FOREACH(const SCycle& l_Cycle, l_CyclesToActivate)
    {
        BlendCycle(l_Cycle, l_fFade);
    }

    m_szAnimationState = _szAnimationState;
}