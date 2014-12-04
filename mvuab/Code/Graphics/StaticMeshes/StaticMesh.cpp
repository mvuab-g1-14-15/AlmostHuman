#include "RenderableVertex\RenderableVertex.h"
#include "RenderableVertex\IndexedVertexs.h"

#include "RenderableVertex\VertexTypes.h"
#include "GraphicsManager.h"
#include "Texture\Texture.h"

#include "Logger\Logger.h"
#include "StaticMesh.h"

#include <cstdio>

CStaticMesh::CStaticMesh()
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

	std::vector<std::vector<CTexture *>> m_Textures;

	for(unsigned int i = 0; i < l_NumMat; i++)
	{
		unsigned short int l_numMatObj = 0;
		std::fread(&l_NumMat, sizeof(unsigned short int), 1, l_pFile);

		std::vector<CTexture *> l_Texture;
		for(unsigned int j = 0; j < l_numMatObj; j++)
		{
			unsigned short l_numLoQueSea = 0; // saber numero de datos a continuacion, no me importa
			std::fread(&l_numLoQueSea, sizeof(unsigned short int), 1, l_pFile);

			unsigned short l_TextureType = 0;
			std::fread(&l_TextureType, sizeof(unsigned short int), 1, l_pFile);

			unsigned short l_TextureLength = 0;
			std::fread(&l_TextureLength, sizeof(unsigned short int), 1, l_pFile);

			char *textureName = (char *) malloc (sizeof(char) * (l_TextureLength + 1));
			ZeroMemory(textureName, l_TextureLength + 1);

			std::fread(textureName, l_TextureLength, 1, l_pFile);
			CTexture *t = new CTexture(); t->Load(textureName);

			unsigned int l_dataSize = 0;
			if(l_TextureType == TNORMAL_TAN_BI_T2_DIFF_VERTEX::GetVertexType()) l_dataSize = sizeof(TNORMAL_TAN_BI_T2_DIFF_VERTEX);
			else if(l_TextureType == TNORMAL_TAN_BI_T1_DIFF_VERTEX::GetVertexType()) l_dataSize = sizeof(TNORMAL_TAN_BI_T1_DIFF_VERTEX);
			
			else if(l_TextureType == TNORMAL_TAN_BI_T2_VERTEX::GetVertexType()) l_dataSize = sizeof(TNORMAL_TAN_BI_T2_VERTEX);
			else if(l_TextureType == TNORMAL_TAN_BI_T1_VERTEX::GetVertexType()) l_dataSize = sizeof(TNORMAL_TAN_BI_T1_VERTEX);

			else if(l_TextureType == TNORMAL_T2_DIFF_VERTEX::GetVertexType()) l_dataSize = sizeof(TNORMAL_T2_DIFF_VERTEX);
			else if(l_TextureType == TNORMAL_T1_DIFF_VERTEX::GetVertexType()) l_dataSize = sizeof(TNORMAL_T1_DIFF_VERTEX);

			else if(l_TextureType == TNORMAL_DIFF_VERTEX::GetVertexType()) l_dataSize = sizeof(TNORMAL_DIFF_VERTEX);
			else if(l_TextureType == TNORMAL_T2_VERTEX::GetVertexType()) l_dataSize = sizeof(TNORMAL_T2_VERTEX);

			else if(l_TextureType == TNORMAL_T1_VERTEX::GetVertexType()) l_dataSize = sizeof(TNORMAL_T1_VERTEX);
			else if(l_TextureType == TT2_DIFF_VERTEX::GetVertexType()) l_dataSize = sizeof(TT2_DIFF_VERTEX);

			else if(l_TextureType == TT1_DIFF_VERTEX::GetVertexType()) l_dataSize = sizeof(TT1_DIFF_VERTEX);
			else if(l_TextureType == TDIFF_VERTEX::GetVertexType()) l_dataSize = sizeof(TDIFF_VERTEX);

			else if(l_TextureType == TT2_VERTEX::GetVertexType()) l_dataSize = sizeof(TT2_VERTEX);
			else if(l_TextureType == TT1_VERTEX::GetVertexType()) l_dataSize = sizeof(TT1_VERTEX);

			l_Texture.push_back(t);
			free(textureName);
		}

		m_Textures.push_back(l_Texture);
	}

	std::fclose(l_pFile);
	return(true);
}

bool CStaticMesh::ReLoad ()
{
	return Load(m_FileName);
}

void CStaticMesh::Render(CGraphicsManager *RM) const
{
}
