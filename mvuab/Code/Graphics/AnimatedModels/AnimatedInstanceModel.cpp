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
#include "Core.h"
#include "Timer\Timer.h"
#include "EngineManagers.h"

#include "XML\XMLTreeNode.h"

#define MAXBONES 29

CAnimatedInstanceModel::CAnimatedInstanceModel( const std::string& Name,
        const std::string& CoreName ) :
    m_CalModel( 0 ),
    m_AnimatedCoreModel( AnimatedMInstance->GetCore( CoreName ) ),
    m_BlendTime( 0.3f ),
    m_LodLevel( 1.0f ),
    m_CurrentAnimationId( 5 ),
    m_NumVtxs( 0 ),
    m_NumFaces( 0 ),
    m_pIB( 0 ),
    m_pVB( 0 ),
    m_ChangeAnimation( 0 ),
    CRenderableObject()
{
    SetName( Name );
    Initialize();
    //m_pEffectTechnique = EffectManagerInstance->GetResource(EffectManagerInstance->GetTechniqueEffectNameByVertexDefault(CAL3D_HW_VERTEX::GetVertexType()));
}
CAnimatedInstanceModel::CAnimatedInstanceModel( CXMLTreeNode& atts )
    : CRenderableObject( atts  )
    , m_CalModel( 0 )
    , m_AnimatedCoreModel( AnimatedMInstance->GetCore( atts.GetPszProperty("core", "" ) ) )
    , m_BlendTime( 0.3f )
    , m_LodLevel( 1.0f )
    , m_CurrentAnimationId( 1 )
    , m_NumVtxs( 0 )
    , m_NumFaces( 0 )
    , m_pIB( 0 )
    , m_pVB( 0 )
    , m_ChangeAnimation(  )
{
    CRenderableObjectTechniqueManager* lROT = ROTMInstance;
    const std::string & l_TechniqueName =
        lROT->GetRenderableObjectTechniqueNameByVertexType( CAL3D_HW_VERTEX::GetVertexType() );
    m_RenderableObjectTechnique = lROT->GetResource( l_TechniqueName );
    Initialize();
}
CAnimatedInstanceModel::~CAnimatedInstanceModel()
{
    Destroy();
}

void CAnimatedInstanceModel::Render()
{
    GraphicsInstance->SetTransform( GetTransform() );
    RenderModelByHardware();
}

void CAnimatedInstanceModel::RenderModelByHardware()
{
    if ( !m_RenderableObjectTechnique )
    { return; }

    // If shaders are reloaded pointers are changing so we have to ask again for the technique
    m_pEffectTechnique = m_RenderableObjectTechnique->GetEffectTechnique();

    if (!m_pEffectTechnique )
    { return; }

    EffectManagerInstance->SetWorldMatrix( GetTransform() );
    CEffect* l_pEffect = m_pEffectTechnique->GetEffect();

    if ( NULL == l_pEffect )
    { return; }

    LPD3DXEFFECT l_Effect = l_pEffect->GetEffect();

    if ( NULL == l_Effect )
    { return; }

    m_pEffectTechnique->BeginRender();
    CalHardwareModel* l_pCalHardwareModel = m_AnimatedCoreModel->GetCalHardwareModel();
    D3DXMATRIX transformation[MAXBONES];

    for ( int hardwareMeshId = 0; hardwareMeshId < l_pCalHardwareModel->getHardwareMeshCount();
            ++hardwareMeshId )
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

        for ( int i = 0; i < l_pCalHardwareModel->getBoneCount(); ++i )
        { memcpy( &l_Matrix[i * 3 * 4], &transformation[i], sizeof( float ) * 3 * 4 ); }

        l_Effect->SetFloatArray( l_pEffect->GetBonesParameter(), ( float* ) l_Matrix,
                                 l_pCalHardwareModel->getBoneCount() * 3 * 4 );
        if( !m_Textures.empty() )
        { m_Textures[0]->Activate( 0 ); }

        //m_NormalTextureList[0]->Activate(1);
        m_AnimatedCoreModel->GetRenderableVertexs()->Render
        (
            GraphicsInstance,
            m_pEffectTechnique,
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
    // get the renderer of the model
    CalRenderer* l_pCalRenderer = m_CalModel->getRenderer();

    // begin the rendering loop
    if ( !l_pCalRenderer->beginRendering() )
    { return; }

    m_VBCursor = 0;
    m_IBCursor = 0;
    DWORD dwVBLockFlags = D3DLOCK_NOOVERWRITE;
    DWORD dwIBLockFlags = D3DLOCK_NOOVERWRITE;
    LPDIRECT3DDEVICE9 l_pD3DDevice = RM->GetDevice();
    l_pD3DDevice->SetStreamSource( 0, m_pVB, 0, sizeof( TNORMAL_T1_VERTEX ) );
    l_pD3DDevice->SetFVF( TNORMAL_T1_VERTEX::GetFVF() );
    l_pD3DDevice->SetIndices( m_pIB );
    // get the number of meshes
    uint32 meshCount = l_pCalRenderer->getMeshCount();

    // render all meshes of the model
    for ( uint32 meshId = 0; meshId < meshCount; ++meshId )
    {
        // get the number of submeshes
        uint32 submeshCount = l_pCalRenderer->getSubmeshCount( meshId );

        // render all submeshes of the mesh
        for ( uint32 submeshId = 0; submeshId < submeshCount; ++submeshId )
        {
            // select mesh and submesh for further data access
            if ( false == l_pCalRenderer->selectMeshSubmesh( meshId, submeshId ) )
            { continue; }

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

            // Get vertexbuffer from the model
            TNORMAL_T1_VERTEX* pVertices;
            m_pVB->Lock( m_VBCursor * sizeof( TNORMAL_T1_VERTEX ),
                         l_pCalRenderer->getVertexCount()*sizeof( TNORMAL_T1_VERTEX ), ( void** )&pVertices, dwVBLockFlags );
            int vertexCount = l_pCalRenderer->getVerticesNormalsAndTexCoords( &pVertices->x );
            m_pVB->Unlock();
            CalIndex* meshFaces;
            int faceCount;
            m_pIB->Lock( m_IBCursor * 3 * sizeof( CalIndex ),
                         l_pCalRenderer->getFaceCount() * 3 * sizeof( CalIndex ), ( void** )&meshFaces, dwIBLockFlags );
            faceCount = l_pCalRenderer->getFaces( meshFaces );
            m_pIB->Unlock();
            l_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
            // Activate textures
            //m_AnimatedCoreModel->ActivateTextures();
            m_Textures[meshId]->Activate( 0 );
            l_pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, m_VBCursor, 0, vertexCount, m_IBCursor * 3,
                                                faceCount );
            m_VBCursor += vertexCount;
            m_IBCursor += faceCount;
            dwIBLockFlags = D3DLOCK_NOOVERWRITE;
            dwVBLockFlags = D3DLOCK_NOOVERWRITE;
        }
    }

    // end the rendering
    l_pCalRenderer->endRendering();
}

void CAnimatedInstanceModel::Update()
{
    m_CalModel->update( deltaTime );
}

void CAnimatedInstanceModel::Initialize()
{
    assert( m_AnimatedCoreModel );
    // Create the calcoremodel
    CalCoreModel* l_CalCoreModel = m_AnimatedCoreModel->GetCoreModel();
    m_CalModel = new CalModel( l_CalCoreModel );
    // attach all meshes to the model
    uint16 l_CoreMeshCount = l_CalCoreModel->getCoreMeshCount();

    for ( uint16 meshId = 0; meshId < l_CoreMeshCount; ++meshId )
    { m_CalModel->attachMesh( meshId ); }

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

    // Load Vertex and Index buffers
    CGraphicsManager* GM = GraphicsInstance;
    LPDIRECT3DDEVICE9 l_pD3DDevice = GM->GetDevice();

    // Create vertex buffer
    if ( FAILED( l_pD3DDevice->CreateVertexBuffer( 30000 * sizeof( TNORMAL_T1_VERTEX ),
                 D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, TNORMAL_T1_VERTEX::GetFVF(), D3DPOOL_DEFAULT , &m_pVB,
                 NULL ) ) )
    { return; }

    // Create index buffer
    if ( sizeof( CalIndex ) == 2 )
    {
        if ( FAILED( l_pD3DDevice->CreateIndexBuffer( 50000 * 3 * sizeof( CalIndex ),
                     D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIB, NULL ) ) )
        { return; }
    }
    else
    {
        if ( FAILED( l_pD3DDevice->CreateIndexBuffer( 50000 * 3 * sizeof( CalIndex ),
                     D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_pIB, NULL ) ) )
        { return; }
    }

    LoadTextures();
    m_AnimatedCoreModel->LoadVertexBuffer( GM );
}

void CAnimatedInstanceModel::Destroy()
{
    CHECKED_DELETE( m_CalModel );
    CHECKED_RELEASE( m_pVB );
    CHECKED_RELEASE( m_pIB );
}

void CAnimatedInstanceModel::ExecuteAction( uint32 Id, float32 DelayIn, float32 DelayOut,
        float32 WeightTarget, bool AutoLock )
{
    m_CalModel->getMixer()->executeAction( Id, DelayIn, DelayOut, WeightTarget, AutoLock );
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
        { return true; }
    }

    return false;
}

bool CAnimatedInstanceModel::IsActionAnimationActive( uint32 Id ) const
{
    const std::vector<CalAnimation*>& l_AnimVect = m_CalModel->getMixer()->getAnimationVector();
    return ( l_AnimVect[Id] != NULL );
}

void CAnimatedInstanceModel::LoadTextures()
{
    for ( size_t i = 0; i < m_AnimatedCoreModel->GetNumTextures(); ++i )
    {
        CTexture* l_Texture = TextureMInstance->GetTexture( m_AnimatedCoreModel->GetTextureName( i ) );
        m_Textures.push_back( l_Texture );
    }
}
