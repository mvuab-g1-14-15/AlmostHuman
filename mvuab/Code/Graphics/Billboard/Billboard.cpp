#include "Billboard.h"
#include "Texture\TextureManager.h"
#include "Texture\Texture.h"
#include "Core.h"
#include "Cameras\CameraManager.h"
#include "Cameras\CameraFPShooter.h"
#include "GraphicsManager.h"
#include "Math\Matrix44.h"
#include "RenderableVertex\VertexTypes.h"
#include "Effects\EffectManager.h"
#include "EngineConfig.h"

CBillboard::CBillboard() : CName()
  , m_Position( Math::Vect3f( 0, 2, 1 ) )
  , m_Size( 2 )
  , m_Active( true )
  , CManager() 
{
}

CBillboard::CBillboard(CXMLTreeNode& atts)
  :CName()
  , m_Position( Math::Vect3f( 0, 2, 1 ) )
  , m_Size( 2 )
  , m_Active( true )
  , CManager(atts)
{
    
}

CBillboard::~CBillboard()
{
}


void CBillboard::Init()
{
  /*
  unsigned short indices[6] = {0, 2, 1, 1, 2, 3};
  TT1_VERTEX v[4] =
  {
    { ul.x,        ul.y,        ul.z, 0,1},
    { dl.x,        dl.y,        dl.z, 1,1},
    { ur.x,        ur.y,        ur.z, 1,0},
    { dr.x,        dr.y,        dr.z, 0,0}
  };
  void* lIdxAddress = malloc( sizeof( unsigned short int ) * 6 );
  void* lVtxsAddress = ( void* ) malloc( 4 * sizeof(TT1_VERTEX) );

  // TODO Copy the  indices and de v into the idxaddres and vtx adress

  mRV = new CIndexedVertexs<TNORMAL_TAN_BI_T1_DIFF_VERTEX>( GraphicsInstance,
          l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount );
  */
   m_Texture = TextureMInstance->GetTexture(mConfigPath );
}

void CBillboard::Update()
{
  if ( m_Active )
  {
    /*Calcular A,B,C,D del rectangulo.
    orientar a la camara (producto vectorial direccion i sumar VectUP) [normalizados]
    N_VectRight=VectUp^VectDir
    a=pos + n_VectorUP*size/2 -N_VectRight*size/2;*/
    CCamera* l_Camera = CameraMInstance->GetCurrentCamera();
    Math::Vect3f l_Pos2Cam = l_Camera->GetPosition() - m_Position;
    Math::Vect3f l_vRight = ( -l_Pos2Cam.Normalize() ) ^ l_Camera->GetVecUp().Normalize();
    l_vRight.Normalize();
    float halfSize = m_Size / 2;
    Math::Vect3f l_newUP( l_Camera->GetVecUp().Normalize()*halfSize );
    Math::Vect3f l_newRight( l_vRight * halfSize );
    m_PosA = m_Position + l_newUP - l_newRight;
    m_PosB = m_Position + l_newUP + l_newRight;
    m_PosC = m_Position - l_newUP - l_newRight;
    m_PosD = m_Position - l_newUP + l_newRight;
  }
}
void CBillboard::Render()
{
  if ( m_Active )
  {
    //GraphicsInstance->EnableAlphaBlend();

    Math::Mat44f l_Mat;
    l_Mat.SetIdentity();
    GraphicsInstance->SetTransform( l_Mat );

    //GraphicsInstance->GetDevice()->SetTexture( 0, m_Texture->GetDXTexture() );
    //GraphicsInstance->GetDevice()->SetFVF( TT1_VERTEX::GetFVF() );

    CEffectTechnique* EffectTechnique = EffectManagerInstance->GetResource( "GenerateGBufferTechnique" );

    CCamera* l_Camera = CameraMInstance->GetCurrentCamera();
    Math::Vect3f l_Pos2Cam = l_Camera->GetPosition() - m_Position;

    GraphicsInstance->DrawQuad3DWithTechnique( m_PosA, m_PosB, m_PosC, m_PosD, l_Pos2Cam.Normalize(), EffectTechnique, m_Texture );

    //GraphicsInstance->GetDevice()->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0, 6, 2,l_Indexes,D3DFMT_INDEX16, l_Points, sizeof( TT1_VERTEX ) );
    //GraphicsInstance->DisableAlphaBlend();
  }
}

void CBillboard::SetTexture( std::string Texture )
{
  m_Texture = TextureMInstance->GetTexture( Texture );
}
CTexture* CBillboard::GetTexture()
{
  return m_Texture;
}