#include "RenderableVertex\RenderableVertex.h"
#include "RenderableVertex\IndexedVertexs.h"

#include "RenderableVertex\VertexTypes.h"
#include "GraphicsManager.h"

#include "Texture\Texture.h"
#include "Texture\TextureManager.h"

#include "Logger\Logger.h"
#include "StaticMesh.h"

#include "Math/AABB.h"

#include "Effects/EffectManager.h"

#include "Core.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"

#include <cstdio>

CStaticMesh::CStaticMesh(): m_FileName( "" ), m_RenderableObjectTechniqueName( "" )
{
}

CStaticMesh::~CStaticMesh()
{
  Destroy();
}

void CStaticMesh::Destroy()
{
  // Clear the references to the vector of textures
  m_Textures.clear();
  m_VertexTypes.clear();

  for ( std::vector<CRenderableVertexs*>::iterator it = m_RVs.begin(); it != m_RVs.end(); ++it )
    CHECKED_DELETE( *it );

  m_RenderableObjectTechniques.clear();
  m_RVs.clear();
}

bool CStaticMesh::Load( const std::string& FileName )
{
  std::FILE* l_pFile = 0;
  m_FileName = FileName;
  fopen_s( &l_pFile, FileName.c_str(), "rb" );

  if ( 0 == l_pFile )
  {
    LOG_ERROR_APPLICATION( "CStaticMesh::Load No se ha podido abrir \"%s\"!",
                           FileName.c_str() );
    return ( false );
  }

  unsigned short int l_header = 0;
  std::fread( &l_header, sizeof( unsigned short int ), 1, l_pFile );

  if ( l_header != 0x55ff )
  {
    std::string error_msg = "CStaticMesh::" + m_FileName + " Load Header incorrecto!";
    LOG_ERROR_APPLICATION( error_msg.c_str() );
    std::fclose( l_pFile );
    return ( false );
  }

  unsigned short int l_SubMeshes = 0;
  std::fread( &l_SubMeshes, sizeof( unsigned short int ), 1, l_pFile );

  for ( unsigned int i = 0; i < l_SubMeshes; ++i )
  {
    // Obtain the vertex type
    unsigned short l_VertexType = 0;
    std::fread( &l_VertexType, sizeof( unsigned short int ), 1, l_pFile );
    // Store it for render purposes
    m_VertexTypes.push_back( l_VertexType );
    // Obtain the size of the vertex
    size_t l_TypeSize = 0;

    if ( l_VertexType == TNORMAL_TAN_BI_T2_DIFF_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TNORMAL_TAN_BI_T2_DIFF_VERTEX );
    else if ( l_VertexType == TNORMAL_TAN_BI_T1_DIFF_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TNORMAL_TAN_BI_T1_DIFF_VERTEX );
    else if ( l_VertexType == TNORMAL_TAN_BI_T2_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TNORMAL_TAN_BI_T2_VERTEX );
    else if ( l_VertexType == TNORMAL_TAN_BI_T1_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TNORMAL_TAN_BI_T1_VERTEX );
    else if ( l_VertexType == TNORMAL_T2_DIFF_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TNORMAL_T2_DIFF_VERTEX );
    else if ( l_VertexType == TNORMAL_T1_DIFF_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TNORMAL_T1_DIFF_VERTEX );
    else if ( l_VertexType == TNORMAL_DIFF_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TNORMAL_DIFF_VERTEX );
    else if ( l_VertexType == TNORMAL_T2_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TNORMAL_T2_VERTEX );
    else if ( l_VertexType == TNORMAL_T1_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TNORMAL_T1_VERTEX );
    else if ( l_VertexType == TT2_DIFF_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TT2_DIFF_VERTEX );
    else if ( l_VertexType == TT1_DIFF_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TT1_DIFF_VERTEX );
    else if ( l_VertexType == TDIFF_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TDIFF_VERTEX );
    else if ( l_VertexType == TT2_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TT2_VERTEX );
    else if ( l_VertexType == TT1_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TT1_VERTEX );
    else if ( l_VertexType == TNORMAL_TAN_BI_DIFF_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TNORMAL_TAN_BI_DIFF_VERTEX );
    else if ( l_VertexType == TNORMAL_T1_REFLECTION_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TNORMAL_T1_REFLECTION_VERTEX );
    else if ( l_VertexType == TRNM_VERTEX::GetVertexType() )
      l_TypeSize = sizeof( TRNM_VERTEX );

    // Obtain all the textures if any
    unsigned short l_numTexturas = 0;
    std::fread( &l_numTexturas, sizeof( unsigned short int ), 1, l_pFile );
    std::vector<CTexture*> l_Texture;

    for ( unsigned int j = 0; j < l_numTexturas; ++j )
    {
      unsigned short l_TextureLength = 0;
      std::fread( &l_TextureLength, sizeof( unsigned short int ), 1, l_pFile );
      ++l_TextureLength;
      char* textureName = ( char* ) malloc( sizeof( char ) * ( l_TextureLength ) );
      ZeroMemory( textureName, sizeof( char )* l_TextureLength );
      std::fread( textureName, sizeof( char ) * l_TextureLength, 1, l_pFile );
      // Get the texture from the texture manager
      CTexture* t = CTextureManager::GetSingletonPtr()->GetTexture( "Data/textures/" + std::string( textureName ) );

      if ( t )
        l_Texture.push_back( t );

      free( textureName );
    }

    m_Textures.push_back( l_Texture );
    // Reading vertex buffer
    unsigned short l_VrtexCount = 0;
    std::fread( &l_VrtexCount, sizeof( unsigned short int ), 1, l_pFile );
    void* l_VtxsAddress = ( void* ) malloc( l_TypeSize * l_VrtexCount );
    std::fread( l_VtxsAddress, l_TypeSize * l_VrtexCount, 1, l_pFile );

    unsigned char* l_AuxVtxAddress = ( unsigned char* )l_VtxsAddress;

    for ( int i = 0; i < l_VrtexCount; i++ )
    {
      Math::Vect3f* l_VtxPointer = ( Math::Vect3f* )l_AuxVtxAddress;
      Math::Vect3f l_Vtx = *l_VtxPointer;
      m_VB.push_back( l_Vtx );
      l_AuxVtxAddress += l_TypeSize;
    }

    // Reading index buffer
    unsigned short l_IdxCount = 0;
    std::fread( &l_IdxCount, sizeof( unsigned short ), 1, l_pFile );
    void* l_IdxAddress = malloc( sizeof( unsigned short int ) * l_IdxCount );
    std::fread( l_IdxAddress, sizeof( unsigned short int ) * l_IdxCount, 1, l_pFile );

    uint16* l_Indexs = ( uint16* )( l_IdxAddress );

    //Vector para physx
    for ( int i = 0; i < l_IdxCount; i++ )
    {
      uint32 l_Idx = l_Indexs[i] + 0;
      m_IB.push_back( l_Idx );
    }


    // Now create the renderable vertex
    CRenderableVertexs* l_RV = NULL;

    if ( l_VertexType == TNORMAL_TAN_BI_T2_DIFF_VERTEX::GetVertexType() )
      l_RV = new CIndexedVertexs<TNORMAL_TAN_BI_T2_DIFF_VERTEX>( GraphicsInstance,
          l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount );
    else if ( l_VertexType == TNORMAL_TAN_BI_T1_DIFF_VERTEX::GetVertexType() )
      l_RV = new CIndexedVertexs<TNORMAL_TAN_BI_T1_DIFF_VERTEX>( GraphicsInstance,
          l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount );
    else if ( l_VertexType == TNORMAL_TAN_BI_T2_VERTEX::GetVertexType() )
    {
      CalcTangentsAndBinormals( l_VtxsAddress, ( unsigned short* )l_IdxAddress, l_VrtexCount, l_IdxCount,
                                l_TypeSize, 0, 12, 28, 44, 60 );
      l_RV = new CIndexedVertexs<TNORMAL_TAN_BI_T2_VERTEX>( GraphicsInstance,
          l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount );
    }
    else if ( l_VertexType == TNORMAL_TAN_BI_T1_VERTEX::GetVertexType() )
    {
      CalcTangentsAndBinormals( l_VtxsAddress, ( unsigned short* )l_IdxAddress, l_VrtexCount, l_IdxCount,
                                l_TypeSize, 0, 12, 28, 44, 60 );
      l_RV = new CIndexedVertexs<TNORMAL_TAN_BI_T1_VERTEX>( GraphicsInstance,
          l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount );
    }
    else if ( l_VertexType == TNORMAL_T2_DIFF_VERTEX::GetVertexType() )
      l_RV = new CIndexedVertexs<TNORMAL_T2_DIFF_VERTEX>( GraphicsInstance,
          l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount );
    else if ( l_VertexType == TNORMAL_T1_DIFF_VERTEX::GetVertexType() )
      l_RV = new CIndexedVertexs<TNORMAL_T1_DIFF_VERTEX>( GraphicsInstance,
          l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount );
    else if ( l_VertexType == TNORMAL_DIFF_VERTEX::GetVertexType() )
      l_RV = new CIndexedVertexs<TNORMAL_DIFF_VERTEX>( GraphicsInstance, l_VtxsAddress,
          l_IdxAddress, l_VrtexCount, l_IdxCount );
    else if ( l_VertexType == TNORMAL_T2_VERTEX::GetVertexType() )
      l_RV = new CIndexedVertexs<TNORMAL_T2_VERTEX>( GraphicsInstance, l_VtxsAddress,
          l_IdxAddress, l_VrtexCount, l_IdxCount );
    else if ( l_VertexType == TNORMAL_T1_VERTEX::GetVertexType() )
      l_RV = new CIndexedVertexs<TNORMAL_T1_VERTEX>( GraphicsInstance, l_VtxsAddress,
          l_IdxAddress, l_VrtexCount, l_IdxCount );
    else if ( l_VertexType == TNORMAL_T1_REFLECTION_VERTEX::GetVertexType() )
      l_RV = new CIndexedVertexs<TNORMAL_T1_REFLECTION_VERTEX>( GraphicsInstance,
          l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount );
    else if ( l_VertexType == TT2_DIFF_VERTEX::GetVertexType() )
      l_RV = new CIndexedVertexs<TT2_DIFF_VERTEX>( GraphicsInstance, l_VtxsAddress,
          l_IdxAddress, l_VrtexCount, l_IdxCount );
    else if ( l_VertexType == TT1_DIFF_VERTEX::GetVertexType() )
      l_RV = new CIndexedVertexs<TT1_DIFF_VERTEX>( GraphicsInstance, l_VtxsAddress,
          l_IdxAddress, l_VrtexCount, l_IdxCount );
    else if ( l_VertexType == TDIFF_VERTEX::GetVertexType() )
      l_RV = new CIndexedVertexs<TDIFF_VERTEX>( GraphicsInstance, l_VtxsAddress,
          l_IdxAddress, l_VrtexCount, l_IdxCount );
    else if ( l_VertexType == TT2_VERTEX::GetVertexType() )
      l_RV = new CIndexedVertexs<TT2_VERTEX>( GraphicsInstance, l_VtxsAddress,
                                              l_IdxAddress, l_VrtexCount, l_IdxCount );
    else if ( l_VertexType == TT1_VERTEX::GetVertexType() )
      l_RV = new CIndexedVertexs<TT1_VERTEX>( GraphicsInstance, l_VtxsAddress,
                                              l_IdxAddress, l_VrtexCount, l_IdxCount );
    else if ( l_VertexType == TNORMAL_TAN_BI_DIFF_VERTEX::GetVertexType() )
      l_RV = new CIndexedVertexs<TNORMAL_TAN_BI_DIFF_VERTEX>( GraphicsInstance,
          l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount );
    else if ( l_VertexType == TRNM_VERTEX::GetVertexType() )
    {
      CalcTangentsAndBinormals( l_VtxsAddress, ( unsigned short* )l_IdxAddress, l_VrtexCount, l_IdxCount,
                                l_TypeSize, 0, 12, 28, 44, 60 );
      l_RV = new CIndexedVertexs<TRNM_VERTEX>( GraphicsInstance,
          l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount );
    }

    // Check the renderable object
    if ( l_RV )
      m_RVs.push_back( l_RV );

    free( l_VtxsAddress );
    free( l_IdxAddress );
  }

  float l_AABB[6];
  std::fread( &l_AABB, sizeof( float ) * 6, 1, l_pFile );
  m_AABB = Math::AABB3f( Math::Vect3f( l_AABB[0], l_AABB[1], l_AABB[2] ), Math::Vect3f( l_AABB[3],
                         l_AABB[4], l_AABB[5] ) );
  unsigned short int l_footer = 0;
  std::fread( &l_footer, sizeof( unsigned short int ), 1, l_pFile );

  if ( l_footer != 0xff55 )
  {
    std::string error_msg = "CStaticMesh::" + m_FileName + " Load Footer incorrecto!";
    LOG_ERROR_APPLICATION( error_msg.c_str() );
    std::fclose( l_pFile );
    return ( false );
  }

  std::fclose( l_pFile );
  return ( GetRenderableObjectTechnique() );
}

bool CStaticMesh::ReLoad()
{
  Destroy();
  return Load( m_FileName );
}

void CStaticMesh::Render( CGraphicsManager* GM )
{
  for ( unsigned int i = 0; i < m_RVs.size(); ++i )
  {
    for ( unsigned int j = 0; j < m_Textures[i].size(); ++j )
      m_Textures[i][j]->Activate( j );

    if ( m_RenderableObjectTechniques[i] != NULL )
      m_RVs[i]->Render( GM, m_RenderableObjectTechniques[i]->GetEffectTechnique() );
    else
      LOG_ERROR_APPLICATION( "No technique in file %s", m_FileName.c_str() );
  }
}

bool CStaticMesh::GetRenderableObjectTechnique()
{
  CRenderableObjectTechniqueManager* l_ROTM =
    CCore::GetSingletonPtr()->GetRenderableObjectTechniqueManager();
  bool l_Ok = true;

  for ( size_t i = 0; i < m_VertexTypes.size(); ++i )
  {
    if ( m_RenderableObjectTechniqueName == "" )
      m_RenderableObjectTechniqueName = l_ROTM->GetRenderableObjectTechniqueNameByVertexType(
                                          m_VertexTypes[i] );

    CRenderableObjectTechnique* l_ROT = l_ROTM->GetResource( m_RenderableObjectTechniqueName );

    if ( l_ROT ) m_RenderableObjectTechniques.push_back( l_ROT );

    if ( l_ROT == NULL )
      LOG_ERROR_APPLICATION(
        "Error trying to GetRenderableObjectTechnique '%s' on CStaticMesh",
        m_RenderableObjectTechniqueName.c_str() );

    l_Ok = l_Ok && l_ROT != NULL;
  }

  return l_Ok;
}