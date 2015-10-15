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


#include "EngineManagers.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"

#include "Materials/Material.h"

#include <cstdio>

namespace
{
    size_t GetVertexSize( unsigned short aVertexType )
    {
             if (aVertexType == TNORMAL_TAN_BI_T2_DIFF_VERTEX::GetVertexType()) { return sizeof(TNORMAL_TAN_BI_T2_DIFF_VERTEX); }
        else if (aVertexType == TNORMAL_TAN_BI_T1_DIFF_VERTEX::GetVertexType()) { return sizeof(TNORMAL_TAN_BI_T1_DIFF_VERTEX); }
        else if (aVertexType == TNORMAL_TAN_BI_T2_VERTEX::GetVertexType())      { return sizeof(TNORMAL_TAN_BI_T2_VERTEX); }
        else if (aVertexType == TNORMAL_TAN_BI_T1_VERTEX::GetVertexType())      { return sizeof(TNORMAL_TAN_BI_T1_VERTEX); }
        else if (aVertexType == TNORMAL_T2_DIFF_VERTEX::GetVertexType())        { return sizeof(TNORMAL_T2_DIFF_VERTEX); }
        else if (aVertexType == TNORMAL_T1_DIFF_VERTEX::GetVertexType())        { return sizeof(TNORMAL_T1_DIFF_VERTEX); }
        else if (aVertexType == TNORMAL_DIFF_VERTEX::GetVertexType())           { return sizeof(TNORMAL_DIFF_VERTEX); }
        else if (aVertexType == TNORMAL_T2_VERTEX::GetVertexType())             { return sizeof(TNORMAL_T2_VERTEX); }
        else if (aVertexType == TNORMAL_T1_VERTEX::GetVertexType())             { return sizeof(TNORMAL_T1_VERTEX); }
        else if (aVertexType == TT2_DIFF_VERTEX::GetVertexType())               { return sizeof(TT2_DIFF_VERTEX); }
        else if (aVertexType == TT1_DIFF_VERTEX::GetVertexType())               { return sizeof(TT1_DIFF_VERTEX); }
        else if (aVertexType == TDIFF_VERTEX::GetVertexType())                  { return sizeof(TDIFF_VERTEX); }
        else if (aVertexType == TT2_VERTEX::GetVertexType())                    { return sizeof(TT2_VERTEX); }
        else if (aVertexType == TT1_VERTEX::GetVertexType() )                   { return sizeof(TT1_VERTEX ); }
        else if (aVertexType == TNORMAL_TAN_BI_DIFF_VERTEX::GetVertexType())    { return sizeof(TNORMAL_TAN_BI_DIFF_VERTEX); }
        else if (aVertexType == TNORMAL_T1_REFLECTION_VERTEX::GetVertexType())  { return sizeof(TNORMAL_T1_REFLECTION_VERTEX); }
        else if (aVertexType == TRNM_VERTEX::GetVertexType())                   { return sizeof(TRNM_VERTEX); }

        return 0;
    }
}

CStaticMesh::CStaticMesh() : m_FileName( "" ), m_RenderableObjectTechniqueName( "" ), m_IsTransformed(false)
{
}

CStaticMesh::~CStaticMesh()
{
    Destroy();
}

void CStaticMesh::Destroy()
{
    // Clear the references to the vector of textures
    for( uint32 i = 0, lCount = mMaterials.size(); i < lCount; ++i )
    {
        CHECKED_DELETE(mMaterials[i]);
    }

    mMaterials.clear();
    m_VertexTypes.clear();

    for (std::vector<CRenderableVertexs*>::iterator it = m_RVs.begin(); it != m_RVs.end(); ++it)
    {
        CHECKED_DELETE( *it );
    }

    m_RenderableObjectTechniques.clear();
    m_RVs.clear();
}

bool CStaticMesh::Load( const std::string& FileName )
{
    bool lOk = false;
    std::FILE* l_pFile = 0;
    m_FileName = FileName;
    fopen_s(&l_pFile, FileName.c_str(), "rb");

    ASSERT(l_pFile, "Null file %s", FileName.c_str() );

    if (l_pFile)
    {
        unsigned short int l_header = 0;
        std::fread(&l_header, sizeof( unsigned short int ), 1, l_pFile);

        if (l_header == 0x55ff)
        {
            unsigned short int l_SubMeshes = 0;
            std::fread(&l_SubMeshes, sizeof(unsigned short int), 1, l_pFile);

            int l_MaxIndex = 0;

            for (unsigned int i = 0; i < l_SubMeshes; ++i)
            {
                // Obtain the vertex type
                unsigned short lVertexType = 0;
                std::fread( &lVertexType, sizeof( unsigned short int ), 1, l_pFile );

                // Store it for render purposes
                m_VertexTypes.push_back( lVertexType );

                // Obtain the size of the vertex
                size_t l_TypeSize = GetVertexSize(lVertexType);

                // Obtain all the textures if any
                unsigned short l_numTexturas = 0;
                std::fread( &l_numTexturas, sizeof( unsigned short int ), 1, l_pFile );
        
                CMaterial* lMaterial = new CMaterial();

                for (unsigned int j = 0; j < l_numTexturas; ++j)
                {
                    unsigned short l_TextureLength = 0;
                    std::fread(&l_TextureLength, sizeof(unsigned short int), 1, l_pFile);
                    ++l_TextureLength;
                    char* textureName = (char *) malloc(sizeof(char) * (l_TextureLength));
                    ZeroMemory(textureName, sizeof(char) * l_TextureLength);
                    std::fread( textureName, sizeof( char ) * l_TextureLength, 1, l_pFile );
                    lMaterial->AddTexture( TextureMInstance->GetTexture( "Data/textures/" + std::string( textureName) ) );
                    free( textureName );
                }

                mMaterials.push_back(lMaterial);

                unsigned int l_VrtexCount = 0;
                std::fread( &l_VrtexCount, sizeof( unsigned int ), 1, l_pFile );

                void *l_VtxsAddress = (void *) malloc (l_TypeSize * l_VrtexCount);
                std::fread(l_VtxsAddress, l_TypeSize * l_VrtexCount, 1, l_pFile);

                // Strider of the vertices
                unsigned char *l_AuxVtxAddress = (unsigned char *) l_VtxsAddress;
                for (unsigned int j = 0; j < l_VrtexCount; ++j)
                {
                    m_VB.push_back(*((Math::Vect3f *) l_AuxVtxAddress));
                    l_AuxVtxAddress += l_TypeSize;
                }

                // Reading index buffer
                unsigned int l_IdxCount = 0;
                std::fread( &l_IdxCount, sizeof( unsigned int ), 1, l_pFile );

                void* l_IdxAddress = malloc( sizeof( unsigned int ) * l_IdxCount );
                std::fread( l_IdxAddress, sizeof( unsigned int ) * l_IdxCount, 1, l_pFile );

                uint32 *l_Indexs = (uint32 *) l_IdxAddress;
                for (unsigned int j = 0; j < l_IdxCount; ++j ) //Vector para physx
                {
                    uint32 l_Idx = l_Indexs[j] + l_MaxIndex;
                    m_IB.push_back(l_Idx);
                }

                for( uint32 j = 0; j < m_IB.size(); ++j )
                {
                    l_MaxIndex = (l_MaxIndex <= (int) m_IB[j]) ? m_IB[j] + 1 : l_MaxIndex;
                }

                // Now create the renderable vertex
                CRenderableVertexs *l_RV = NULL;

                if (lVertexType == TNORMAL_TAN_BI_T2_DIFF_VERTEX::GetVertexType())
                { l_RV = new CIndexedVertexs<TNORMAL_TAN_BI_T2_DIFF_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount); }
                else if (lVertexType == TNORMAL_TAN_BI_T1_DIFF_VERTEX::GetVertexType() )
                { l_RV = new CIndexedVertexs<TNORMAL_TAN_BI_T1_DIFF_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount); }
                else if (lVertexType == TNORMAL_TAN_BI_T2_VERTEX::GetVertexType())
                {
                    CalcTangentsAndBinormals(l_VtxsAddress, (unsigned short *)l_IdxAddress, l_VrtexCount, l_IdxCount, l_TypeSize, 0, 12, 28, 44, 60);
                    l_RV = new CIndexedVertexs<TNORMAL_TAN_BI_T2_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);
                }
                else if (lVertexType == TNORMAL_TAN_BI_T1_VERTEX::GetVertexType())
                {
                    CalcTangentsAndBinormals(l_VtxsAddress, (unsigned short *)l_IdxAddress, l_VrtexCount, l_IdxCount, l_TypeSize, 0, 12, 28, 44, 60);
                    l_RV = new CIndexedVertexs<TNORMAL_TAN_BI_T1_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);
                }
                else if (lVertexType == TNORMAL_T2_DIFF_VERTEX::GetVertexType())
                { l_RV = new CIndexedVertexs<TNORMAL_T2_DIFF_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount); }
                else if (lVertexType == TNORMAL_T1_DIFF_VERTEX::GetVertexType())
                { l_RV = new CIndexedVertexs<TNORMAL_T1_DIFF_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount); }
                else if (lVertexType == TNORMAL_DIFF_VERTEX::GetVertexType())
                { l_RV = new CIndexedVertexs<TNORMAL_DIFF_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount); }
                else if (lVertexType == TNORMAL_T2_VERTEX::GetVertexType())
                { l_RV = new CIndexedVertexs<TNORMAL_T2_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount); }
                else if (lVertexType == TNORMAL_T1_VERTEX::GetVertexType())
                { l_RV = new CIndexedVertexs<TNORMAL_T1_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount); }
                else if (lVertexType == TNORMAL_T1_REFLECTION_VERTEX::GetVertexType())
                { l_RV = new CIndexedVertexs<TNORMAL_T1_REFLECTION_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount); }
                else if (lVertexType == TT2_DIFF_VERTEX::GetVertexType())
                { l_RV = new CIndexedVertexs<TT2_DIFF_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount); }
                else if (lVertexType == TT1_DIFF_VERTEX::GetVertexType())
                { l_RV = new CIndexedVertexs<TT1_DIFF_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount); }
                else if (lVertexType == TDIFF_VERTEX::GetVertexType())
                { l_RV = new CIndexedVertexs<TDIFF_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount); }
                else if (lVertexType == TT2_VERTEX::GetVertexType())
                { l_RV = new CIndexedVertexs<TT2_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount); }
                else if (lVertexType == TT1_VERTEX::GetVertexType())
                { l_RV = new CIndexedVertexs<TT1_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount); }
                else if (lVertexType == TNORMAL_TAN_BI_DIFF_VERTEX::GetVertexType())
                { l_RV = new CIndexedVertexs<TNORMAL_TAN_BI_DIFF_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount); }
                else if (lVertexType == TRNM_VERTEX::GetVertexType())
                {
                    CalcTangentsAndBinormals(l_VtxsAddress, (unsigned short *)l_IdxAddress, l_VrtexCount, l_IdxCount, l_TypeSize, 0, 12, 28, 44, 60);
                    l_RV = new CIndexedVertexs<TRNM_VERTEX, int>(GraphicsInstance, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);
                }

                // Check the renderable object
                if (l_RV && l_RV->isRenderOK()) m_RVs.push_back(l_RV);
                else CHECKED_DELETE(l_RV);

                free(l_VtxsAddress);
                free(l_IdxAddress);
            }

            float l_AABB[6];
            std::fread(&l_AABB, sizeof(float) * 6, 1, l_pFile);
            m_AABB = Math::AABB3f(Math::Vect3f(l_AABB[0], l_AABB[1], l_AABB[2] ), Math::Vect3f( l_AABB[3], l_AABB[4], l_AABB[5]));

            unsigned short int l_footer = 0;
            std::fread( &l_footer, sizeof( unsigned short int ), 1, l_pFile );

            lOk = (l_footer == 0xff55);
        }
    
        std::fclose( l_pFile );
    }

    lOk = lOk && GetRenderableObjectTechnique();
    lOk = lOk && m_RVs.size() == m_RenderableObjectTechniques.size();
    ASSERT( lOk, "Null EffectTechnique for the static mesh %s", m_FileName.c_str() );
    return lOk;
}

bool CStaticMesh::ReLoad()
{
    Destroy();
    return Load( m_FileName );
}

void CStaticMesh::Render( CGraphicsManager* GM )
{
    for(uint32 i = 0; i < m_RVs.size(); ++i)
    {
        CEffectTechnique* lEffecTch = m_RenderableObjectTechniques[i]->GetEffectTechnique();
        mMaterials[i]->Apply( lEffecTch );
        m_RVs[i]->Render( GM, m_RenderableObjectTechniques[i]->GetEffectTechnique() );
    }
}

bool CStaticMesh::GetRenderableObjectTechnique()
{
    CRenderableObjectTechniqueManager* l_ROTM = ROTMInstance;
    bool l_Ok = true;

    for ( size_t i = 0; i < m_VertexTypes.size(); ++i )
    {
        if (m_RenderableObjectTechniqueName == "") { m_RenderableObjectTechniqueName = l_ROTM->GetRenderableObjectTechniqueNameByVertexType(m_VertexTypes[i]); }
        CRenderableObjectTechnique* l_ROT = l_ROTM->GetResource(m_RenderableObjectTechniqueName);

        if (l_ROT) { m_RenderableObjectTechniques.push_back(l_ROT); }
        else { LOG_ERROR_APPLICATION("Error trying to GetRenderableObjectTechnique '%s' on CStaticMesh", m_RenderableObjectTechniqueName.c_str()); }

        l_Ok = l_Ok && l_ROT != NULL;
    }

    return l_Ok;
}
