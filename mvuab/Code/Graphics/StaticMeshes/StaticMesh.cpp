#include "RenderableVertex\RenderableVertex.h"
#include "RenderableVertex\IndexedVertexs.h"

#include "RenderableVertex\VertexTypes.h"
#include "GraphicsManager.h"
#include "Texture\Texture.h"

#include "Logger\Logger.h"
#include "StaticMesh.h"

#include <cstdio>

CStaticMesh::CStaticMesh(): m_FileName(""), m_NumFaces(0), m_NumVertexs(0)
{
}

CStaticMesh::~CStaticMesh()
{
	for(std::vector<std::vector<CTexture *>>::iterator it1 = m_Textures.begin(); it1 != m_Textures.end(); it1++)
	{
		for(std::vector<CTexture *>::iterator it2 = it1->begin(); it2 != it1->end(); it2++) delete(*it2); 
		it1->clear();
	}

	m_Textures.clear();
}

bool CStaticMesh::Load(const std::string &FileName)
{
	std::FILE *l_pFile = 0;
	m_FileName = FileName;
	m_Textures.clear();

	if(0 == (l_pFile = std::fopen(FileName.c_str(), "rb")))
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

			CTexture *t = new CTexture(); 
			if(!t->Load(textureName)) delete (t);
			l_Texture.push_back(t);
		}
		m_Textures.push_back(l_Texture);
	}

	CGraphicsManager *pGraphicsManager = CGraphicsManager::GetSingletonPtr();

	for(unsigned int i = 0; i < l_NumMat; ++i)
	{
		// Reading vertex buffer
		unsigned short l_VrtexCount = 0;
		std::fread(&l_VrtexCount, sizeof(unsigned short int), 1, l_pFile);
		
		void *l_VtxsAddress=NULL;
		size_t l_dataSize;
		
		if(l_VertexTypes[i] == TNORMAL_T1_VERTEX::GetVertexType()) 
			l_dataSize = sizeof(TNORMAL_T1_VERTEX)*l_VrtexCount;
		l_VtxsAddress=malloc(l_dataSize);

		std::fread(l_VtxsAddress, l_dataSize, 1, l_pFile);

		// Reading index buffer
		unsigned short l_IdxCount = 0;
		std::fread(&l_IdxCount, sizeof(unsigned short int), 1, l_pFile);
		
		void *l_IdxAddress=NULL;
		l_IdxAddress=malloc(sizeof(unsigned short)*l_IdxCount);

		std::fread(l_VtxsAddress, sizeof(unsigned short), 1, l_pFile);

		// Now create the renderable vertex
		CRenderableVertexs *l_RV=NULL;
		if(l_VertexTypes[i] == TNORMAL_T1_VERTEX::GetVertexType()) 		
		{
			l_RV=new CIndexedVertexs<TNORMAL_T1_VERTEX>(pGraphicsManager, l_VtxsAddress, l_IdxAddress, l_VrtexCount, l_IdxCount);
		}

		// Check the renderable object
		if(l_RV)
			m_RVs.push_back(l_RV);
	}

	std::fclose(l_pFile);
	return(true);
}

bool CStaticMesh::ReLoad ()
{
	return Load(m_FileName);
}

void CStaticMesh::Render(CGraphicsManager *GM) const
{
	for(unsigned int i=0; i<m_RVs.size(); ++i)
		m_RVs[i]->Render(GM);
}
