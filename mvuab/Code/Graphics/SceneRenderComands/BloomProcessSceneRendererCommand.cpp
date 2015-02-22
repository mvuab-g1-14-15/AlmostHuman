#include "BloomProcessSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"
#include "Effects\EffectTechnique.h"
#include "Texture\Texture.h"
#include "Math\Vector2.h"
#include "Math\MathUtils.h"

#include <d3dx9.h>

CBloomProcessSceneRenderCommand::CBloomProcessSceneRenderCommand( const SBloomProcessSettings&
    settings )
  : m_Settings( settings )
  , m_Scene( NULL )
  , m_RT1( NULL )
  , m_RT2( NULL )
  , m_BloomExtract( NULL )
  , m_GaussianBlur( NULL )
  , m_BloomCombine( NULL )
  , m_SceneTextureParameter( NULL )
  , m_BloomThresholdParameter( NULL )
  , m_SampleOffsetsParameter( NULL )
  , m_SampleWeightsParameter( NULL )
  , m_GaussianBlurTextureParameter( NULL )
  , m_BloomIntensityParameter( NULL )
  , m_BaseIntensityParameter( NULL )
  , m_BloomSaturationParameter( NULL )
  , m_BaseSaturationParameter( NULL )
  , m_PostBloomTextureParameter( NULL )
  , m_BloomEffect( NULL )
{
}

CBloomProcessSceneRenderCommand::~CBloomProcessSceneRenderCommand()
{
  CHECKED_DELETE( m_Scene );
  CHECKED_DELETE( m_RT1 );
  CHECKED_DELETE( m_RT2 );
}

void CBloomProcessSceneRenderCommand::Init()
{
  //3 Techniques to apply
  m_BloomExtract = CEffectManager::GetSingletonPtr()->GetResource( "BloomFilter" );
  m_BloomCombine = CEffectManager::GetSingletonPtr()->GetResource( "BloomCombine" );
  m_GaussianBlur = CEffectManager::GetSingletonPtr()->GetResource( "GaussianBlur" );

  //Effect to apply
  m_BloomEffect =  CEffectManager::GetSingletonPtr()->GetEffect( "BloomProcessEffect" );

  //Values of handlers.
  m_BloomEffect->GetEffect()->GetParameterBySemantic( "SCENE_TEXTURE", m_SceneTextureParameter );
  m_BloomEffect->GetEffect()->GetParameterBySemantic( "BLOOM_THRESHOLD", m_BloomThresholdParameter );
  m_BloomEffect->GetEffect()->GetParameterBySemantic( "SAMPLE_OFFSETS", m_SampleOffsetsParameter );
  m_BloomEffect->GetEffect()->GetParameterBySemantic( "SAMPLE_WEIGHTS", m_SampleWeightsParameter );
  m_BloomEffect->GetEffect()->GetParameterBySemantic( "GAUSSIAN_TEXTURE", m_GaussianBlurTextureParameter );
  m_BloomEffect->GetEffect()->GetParameterBySemantic( "BLOOM_INTENSITY", m_BloomIntensityParameter );
  m_BloomEffect->GetEffect()->GetParameterBySemantic( "BASE_INTENSITY", m_BaseIntensityParameter );
  m_BloomEffect->GetEffect()->GetParameterBySemantic( "BLOOM_SATURATION", m_BloomSaturationParameter );
  m_BloomEffect->GetEffect()->GetParameterBySemantic( "BASE_SATURATION", m_BaseSaturationParameter );
  m_BloomEffect->GetEffect()->GetParameterBySemantic( "POST_BLOOM_TEXTURE", m_PostBloomTextureParameter );

  uint32 width, height;
  CGraphicsManager::GetSingletonPtr()->GetWidthAndHeight( width, height );

  m_Scene = new CTexture();
  m_Scene->Load( "bloom_test.jpg" );
  //m_Scene->Create( "SCENE_TEXTURE", width, height, 1, CTexture::RENDERTARGET, CTexture::DEFAULT,CTexture::A8R8G8B8 );
  width /= 2;
  height /= 2;

  m_RT1 = new CTexture();
  m_RT1->Create( "RT1", width, height, 1, CTexture::RENDERTARGET, CTexture::DEFAULT,
                 CTexture::A8R8G8B8 );

  m_RT2 = new CTexture();
  m_RT2->Create( "RT2", width, height, 1, CTexture::RENDERTARGET, CTexture::DEFAULT,
                 CTexture::A8R8G8B8 );
}

void CBloomProcessSceneRenderCommand::Render()
{
  m_Scene->CaptureFrameBuffer( 0 );
  RenderBloomExtract();
  RenderBloomGaussianBlur();
  RenderBloomCombine();
}

void CBloomProcessSceneRenderCommand::SetBlurParameters( float dx, float dy )
{
  float sampleWeights[BLOOM_BLUR_SAMPLE_COUNT];
  Math::Vect2f sampleOffsets[BLOOM_BLUR_SAMPLE_COUNT];
  sampleWeights[0] = ComputeGaussianBlur( 0 );
  sampleOffsets[0] = Math::v2fZERO;
  float totalWeights = sampleWeights[0];

  for ( int i = 0; i < BLOOM_BLUR_SAMPLE_COUNT / 2; i++ )
  {

    float weight = ComputeGaussianBlur( i + 1.0f );
    sampleWeights[i * 2 + 1] = weight;
    sampleWeights[i * 2 + 2] = weight;
    totalWeights += weight * 2;
    float sampleOffset = i * 2 + 1.5f;
    Math::Vect2f delta = Math::Vect2f( dx, dy ) * sampleOffset;
    sampleOffsets[i * 2 + 1] = delta;
    sampleOffsets[i * 2 + 2] = -( delta );
  }

  // Normalize the list of sample weightings, so they will always sum to one.
  for ( int i = 0; i < BLOOM_BLUR_SAMPLE_COUNT; i++ )
    sampleWeights[i] /= totalWeights;

  // Tell the effect about our new filter settings.
  m_BloomEffect->GetEffect()->SetFloatArray( m_SampleWeightsParameter, sampleWeights,
      BLOOM_BLUR_SAMPLE_COUNT );
  m_BloomEffect->GetEffect()->SetFloatArray( m_SampleOffsetsParameter, ( float* )sampleOffsets,
      BLOOM_BLUR_SAMPLE_COUNT * 2 );
}

float CBloomProcessSceneRenderCommand::ComputeGaussianBlur( float n )
{
  float theta = m_Settings.m_BlurAmount;
  return ( float )( ( 1.0 / sqrt( 2 * D3DX_PI * theta ) ) *
                    exp( -( n * n ) / ( 2 * theta * theta ) ) );
}

bool CBloomProcessSceneRenderCommand::RenderBloomExtract()
{
  m_RT1->SetAsRenderTarget( 0 );
  m_BloomEffect->GetEffect()->SetTexture( m_SceneTextureParameter, m_Scene->GetDXTexture() );
  m_BloomEffect->GetEffect()->SetFloat( m_BloomThresholdParameter, m_Settings.m_BloomThreshold );
  CGraphicsManager::GetSingletonPtr()->DrawQuad2DTexturedInPixelsInFullScreen( m_BloomExtract );
  m_RT1->UnsetAsRenderTarget( 0 );
  return true;
}

bool CBloomProcessSceneRenderCommand::RenderBloomGaussianBlur()
{
  m_RT2->SetAsRenderTarget( 0 );
  SetBlurParameters( 1.0f / ( float )m_RT1->GetWidth(), 0.0f );
  m_BloomEffect->GetEffect()->SetTexture( m_GaussianBlurTextureParameter, m_RT1->GetDXTexture() );
  CGraphicsManager::GetSingletonPtr()->DrawQuad2DTexturedInPixelsInFullScreen( m_GaussianBlur );
  m_RT2->UnsetAsRenderTarget( 0 );
  //Do Gaussian Blur Vertical
  m_RT1->SetAsRenderTarget( 0 );
  SetBlurParameters( 0.0f, 1.0f / ( float )m_RT1->GetHeight() );
  m_BloomEffect->GetEffect()->SetTexture( m_GaussianBlurTextureParameter, m_RT2->GetDXTexture() );
  CGraphicsManager::GetSingletonPtr()->DrawQuad2DTexturedInPixelsInFullScreen( m_GaussianBlur );
  m_RT1->UnsetAsRenderTarget( 0 );
  return true;
}

bool CBloomProcessSceneRenderCommand::RenderBloomCombine()
{
  m_BloomEffect->GetEffect()->SetTexture( m_SceneTextureParameter, m_Scene->GetDXTexture() );
  m_BloomEffect->GetEffect()->SetTexture( m_PostBloomTextureParameter, m_RT1->GetDXTexture() );
  m_BloomEffect->GetEffect()->SetFloat( m_BloomIntensityParameter, m_Settings.m_BloomIntensity );
  m_BloomEffect->GetEffect()->SetFloat( m_BaseIntensityParameter, m_Settings.m_BaseIntensity );
  m_BloomEffect->GetEffect()->SetFloat( m_BloomSaturationParameter, m_Settings.m_BloomSaturation );
  m_BloomEffect->GetEffect()->SetFloat( m_BaseSaturationParameter, m_Settings.m_BaseSaturation );
  CGraphicsManager::GetSingletonPtr()->DrawQuad2DTexturedInPixelsInFullScreen( m_BloomCombine );
  return true;
}
