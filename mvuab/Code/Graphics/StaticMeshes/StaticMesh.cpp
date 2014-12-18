#include "RenderableVertex\RenderableVertex.h"
#include "RenderableVertex\IndexedVertexs.h"

#include "RenderableVertex\VertexTypes.h"
#include "GraphicsManager.h"

#include "Texture\Texture.h"
#include "Texture\TextureManager.h"

#include "Logger\Logger.h"
#include "StaticMesh.h"

#include "Math/AABB.h"

#include <cstdio>

CStaticMesh::CStaticMesh(): m_FileName(""), m_NumFaces(0), m_NumVertexs(0), m_NumDraws(0)
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

    for(std::vector<CRenderableVertexs*>::iterator it = m_RVs.begin(); it != m_RVs.end(); ++it)
    {
        CHECKED_DELETE(*it);
    }
    m_RVs.clear();
}

bool CStaticMesh::Load(const std::string &FileName)
{
    std::FILE *l_pFile = 0;
    m_FileName = FileName;

    fopen_s(&l_pFile, FileName.c_str(), "rb");
    if(0 == l_pFile)
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CStaticMesh::Load No se ha podido abrir \"%s\"!", FileName.c_str());
        return(false);
    }

    unsigned short int l_header = 0;
    std::fread(&l_header, sizeof(unsigned short int), 1, l_pFile);

    if(l_header != 0x55ff)
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CStaticMesh::Load Header incorrecto!");
        std::fclose(l_pFile); return(false);
    }

    unsigned short int l_NumMat = 0;
    std::fread(&l_NumMat, sizeof(unsigned short int), 1, l_pFile);

    std::vector<unsigned int> l_VertexTypes;
    for(unsigned int i = 0; i < l_NumMat; ++i)
    {
        unsigned short l_VertexType = 0;
        std::fread(&l_VertexType, sizeof(unsigned short int), 1, l_pFile);

        unsigned short int l_numTexturas = 1;

        std::vector<CTexture *> l_Texture;
        for(unsigned int j = 0; j < l_numTexturas; ++j)
        {
            l_VertexTypes.push_back(l_VertexType);

            unsigned short l_TextureLength = 0;
            std::fread(&l_TextureLength, sizeof(unsigned short int), 1, l_pFile);
            ++l_TextureLength;

            char *textureName = (char *) malloc (sizeof(char) * (l_TextureLength));
            ZeroMemory(textureName, sizeof(char)* l_TextureLength);

            std::fread(textureName, sizeof(char) * l_TextureLength, 1, l_pFile);

            // Get the texture from the texture manager
            CTexture *t = CTextureManager::GetSingletonPtr()->GetTexture(textureName);
            if(t)
            {
                l_Texture.push_back(t);
            }

            free(textureName);
        }
        m_Textures.push_back(l_Texture);
    }

    for(unsigned int i = 0; i < l_NumMat; ++i)
    {
        // Reading vertex buffer
        unsigned short l_VrtexCount = 0;
        std::fread(&l_VrtexCount, sizeof(unsigned short int), 1, l_pFile);
        
        void *l_VtxsAddress = NULL;
        size_t l_dataSize = 0;
        
        if(l_VertexTypes[i] == TNORMAL_TAN_BI_T2_DIFF_VERTEX::GetVertexType()) 
            l_dataSize = sizeof(TNORMAL_TAN_BI_T2_DIFF_VERTEX) * l_VrtexCount;

        else if(l_VertexTypes[i] == TNORMAL_TAN_BI_T1_DIFF_VERTEX::GetVertexType()) 
            l_dataSize = sizeof(TNORMAL_TAN_BI_T1_DIFF_VERTEX) * l_VrtexCount;

        else if(l_VertexTypes[i] == TNORMAL_TAN_BI_T2_VERTEX::GetVertexType()) 
            l_dataSize = sizeof(TNORMAL_TAN_BI_T2_VERTEX) * l_VrtexCount;

        else if(l_VertexTypes[i] == TNORMAL_TAN_BI_T1_VERTEX::GetVertexType()) 
            l_dataSize = sizeof(TNORMAL_TAN_BI_T1_VERTEX) * l_VrtexCount;
        
        else if(l_VertexTypes[i] == TNORMAL_T2_DIFF_VERTEX::GetVertexType()) 
            l_dataSize = sizeof(TNORMAL_T2_DIFF_VERTEX) * l_VrtexCount;

        else if(l_VertexTypes[i] == TNORMAL_T1_DIFF_VERTEX::GetVertexType()) 
            l_dataSize = sizeof(TNORMAL_T1_DIFF_VERTEX) * l_VrtexCount;
        
        else if(l_VertexTypes[i] == TNORMAL_DIFF_VERTEX::GetVertexType()) 
            l_dataSize = sizeof(TNORMAL_DIFF_VERTEX) * l_VrtexCount;

        else if(l_VertexTypes[i] == TNORMAL_T2_VERTEX::GetVertexType()) 
            l_dataSize = sizeof(TNORMAL_T2_VERTEX) * l_VrtexCount;
        
        else if(l_VertexTypes[i] == TNORMAL_T1_VERTEX::GetVertexType()) 
            l_dataSize = sizeof(TNORMAL_T1_VERTEX) * l_VrtexCount;

        else if(l_VertexTypes[i] == TT2_DIFF_VERTEX::GetVertexType()) 
            l_dataSize = sizeof(TT2_DIFF_VERTEX) * l_VrtexCount;

        else if(l_VertexTypes[i] == TT1_DIFF_VERTEX::GetVertexType()) 
            l_dataSize = sizeof(TT1_DIFF_VERTEX) * l_VrtexCount;

        else if(l_VertexTypes[i] == TDIFF_VERTEX::GetVertexType()) 
            l_dataSize = sizeof(TDIFF_VERTEX) * l_VrtexCount;

        else if(l_VertexTypes[i] == TT2_VERTEX::GetVertexType()) 
            l_dataSize = sizeof(TT2_VERTEX) * l_VrtexCount;

        else if(l_VertexTypes[i] == TT1_VERTEX::GetVertexType()) 
            l_dataSize = sizeof(TT1_VERTEX) * l_VrtexCount;

        l_VtxsAddress = (void *) malloc(l_dataSize);
        std::fread(l_VtxsAddress, l_dataSize, 1, l_pFile);

        // Reading index buffer
        unsigned short l_IdxCount = 0;
        std::fread(&l_IdxCount, sizeof(unsigned short), 1, l_pFile);
        
        void *l_IdxAddress = NULL;
        l_IdxAddress = malloc(sizeof(unsigned short int) * l_IdxCount);
        std::fread(l_IdxAddress, sizeof(unsigned short int) * l_IdxCount, 1, l_pFile);

        // Now create the renderable vertex
        CRenderableVertexs *l_RV = NULL;

        if(l_VertexTypes[i] == TNORMAL_TAN_BI_T2_DIFF_VERTEX::GetVertexType()) 
            l_RV = new CIndexedVertexs<TNORMAL_TAN_BI_T2_DIFF_VERTEX>(CGraphicsManager::GetSingletonPtr(), l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);
        
        else if(l_VertexTypes[i] == TNORMAL_TAN_BI_T1_DIFF_VERTEX::GetVertexType()) 
            l_RV = new CIndexedVertexs<TNORMAL_TAN_BI_T1_DIFF_VERTEX>(CGraphicsManager::GetSingletonPtr(), l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);

        else if(l_VertexTypes[i] == TNORMAL_TAN_BI_T2_VERTEX::GetVertexType()) 
            l_RV = new CIndexedVertexs<TNORMAL_TAN_BI_T2_VERTEX>(CGraphicsManager::GetSingletonPtr(), l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);
        
        else if(l_VertexTypes[i] == TNORMAL_TAN_BI_T1_VERTEX::GetVertexType()) 
            l_RV = new CIndexedVertexs<TNORMAL_TAN_BI_T1_VERTEX>(CGraphicsManager::GetSingletonPtr(), l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);
        
        else if(l_VertexTypes[i] == TNORMAL_T2_DIFF_VERTEX::GetVertexType())  
            l_RV = new CIndexedVertexs<TNORMAL_T2_DIFF_VERTEX>(CGraphicsManager::GetSingletonPtr(), l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);
       
        else if(l_VertexTypes[i] == TNORMAL_T1_DIFF_VERTEX::GetVertexType()) 
            l_RV = new CIndexedVertexs<TNORMAL_T1_DIFF_VERTEX>(CGraphicsManager::GetSingletonPtr(), l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);
        
        else if(l_VertexTypes[i] == TNORMAL_DIFF_VERTEX::GetVertexType()) 
            l_RV = new CIndexedVertexs<TNORMAL_DIFF_VERTEX>(CGraphicsManager::GetSingletonPtr(), l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);
        
        else if(l_VertexTypes[i] == TNORMAL_T2_VERTEX::GetVertexType()) 
            l_RV = new CIndexedVertexs<TNORMAL_T2_VERTEX>(CGraphicsManager::GetSingletonPtr(), l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);
        
        else if(l_VertexTypes[i] == TNORMAL_T1_VERTEX::GetVertexType()) 
            l_RV = new CIndexedVertexs<TNORMAL_T1_VERTEX>(CGraphicsManager::GetSingletonPtr(), l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);
        
        else if(l_VertexTypes[i] == TT2_DIFF_VERTEX::GetVertexType()) 
            l_RV = new CIndexedVertexs<TT2_DIFF_VERTEX>(CGraphicsManager::GetSingletonPtr(), l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);

        else if(l_VertexTypes[i] == TT1_DIFF_VERTEX::GetVertexType()) 
            l_RV = new CIndexedVertexs<TT1_DIFF_VERTEX>(CGraphicsManager::GetSingletonPtr(), l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);
        
        else if(l_VertexTypes[i] == TDIFF_VERTEX::GetVertexType()) 
            l_RV = new CIndexedVertexs<TDIFF_VERTEX>(CGraphicsManager::GetSingletonPtr(), l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);

        else if(l_VertexTypes[i] == TT2_VERTEX::GetVertexType()) 
            l_RV = new CIndexedVertexs<TT2_VERTEX>(CGraphicsManager::GetSingletonPtr(), l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);
        
        else if(l_VertexTypes[i] == TT1_VERTEX::GetVertexType()) 
            l_RV = new CIndexedVertexs<TT1_VERTEX>(CGraphicsManager::GetSingletonPtr(), l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);

        // Check the renderable object
        if(l_RV) m_RVs.push_back(l_RV);
        free(l_VtxsAddress); 
        free(l_IdxAddress);
    }

	float l_AABB[6];
    std::fread(&l_AABB, sizeof(float) * 6, 1, l_pFile);

  	m_AABB = Math::AABB3f(Math::Vect3f(l_AABB[0], l_AABB[1], l_AABB[2]), Math::Vect3f(l_AABB[3], l_AABB[4], l_AABB[5]));

    unsigned short int l_footer = 0;
    std::fread(&l_footer, sizeof(unsigned short int), 1, l_pFile);

    if(l_footer != 0xff55)
    {
        CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CStaticMesh::Load Footer incorrecto!");
        std::fclose(l_pFile); return(false);
    }

    std::fclose(l_pFile);
    return(true);
}

bool CStaticMesh::ReLoad ()
{
    Destroy();
    return Load(m_FileName);
}

void CStaticMesh::Render(CGraphicsManager *GM)
{
    m_NumVertexs = m_NumFaces = m_NumDraws = 0;

    for(unsigned int i = 0; i < m_RVs.size(); ++i)
    {
        for(unsigned int j = 0; j < m_Textures[i].size(); ++j)
        {
            m_Textures[i][j]->Activate(j);
        }

        m_NumVertexs += m_RVs[i]->GetVertexsCount();
        m_NumFaces += m_RVs[i]->GetFacesCount();
        ++m_NumDraws;

        m_RVs[i]->Render(GM);
    }
}
