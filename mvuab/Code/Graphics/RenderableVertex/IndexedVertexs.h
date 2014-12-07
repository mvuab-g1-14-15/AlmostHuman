#ifndef _INDEX_VERTEXS_H
#define _INDEX_VERTEXS_H
#pragma once

#include "RenderableVertex.h"

template<class T> class CIndexedVertexs : public CRenderableVertexs
{
	protected:
		inline size_t GetVertexSize() {return sizeof(T);}
		inline size_t GetIndexSize() {return sizeof(unsigned short);}

	public:
		CIndexedVertexs(CGraphicsManager *GM, void *VertexAddress, void *IndexAddres, size_t VertexCount, size_t IndexCount)
		{
			void *l_memSrcV = 0; 
			void *l_memSrcI = 0;

			m_IndexCount = IndexCount;
			m_VertexCount = VertexCount;

			if(S_OK != GM->GetDevice()->CreateIndexBuffer(IndexCount * GetIndexSize(), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_IB, 0))
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CIndexedVertexs::CIndexedVertexs CreateIndexBuffer error!");
            }

			if(S_OK != GM->GetDevice()->CreateVertexBuffer(VertexCount * GetVertexSize(), 0, T::GetFVF(), D3DPOOL_DEFAULT, &m_VB, 0))
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CIndexedVertexs::CIndexedVertexs CreateVertexBuffer error!");
            }
			
			if(S_OK != m_VB->Lock(0, VertexCount * GetVertexSize(), &l_memSrcV, 0))
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CIndexedVertexs::CIndexedVertexs m_VB->Lock error!");
            }

			memcpy(l_memSrcV, VertexAddress, VertexCount * GetVertexSize());

			if(S_OK != m_VB->Unlock())
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CIndexedVertexs::CIndexedVertexs m_VB->Unlock error!");
            }

			if(S_OK != m_IB->Lock(0, IndexCount * GetIndexSize(), &l_memSrcI, 0))
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CIndexedVertexs::CIndexedVertexs m_IB->Lock error!");
            }

			memcpy(l_memSrcI, IndexAddres, IndexCount * GetIndexSize());

			if(S_OK != m_IB->Unlock())
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CIndexedVertexs::CIndexedVertexs m_IB->Unlock error!");
            }
		}

		~CIndexedVertexs()
		{
		}

		virtual bool Render(CGraphicsManager *GM)
		{
			if(S_OK != GM->GetDevice()->SetStreamSource(0, m_VB, 0, GetVertexSize()))
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CIndexedVertexs::Render SetStreamSource error!");
                return(false);
            }

			if(S_OK != GM->GetDevice()->SetIndices(m_IB))
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CIndexedVertexs::Render SetIndices error!");
                return(false);
            }

			if(S_OK != GM->GetDevice()->SetFVF(T::GetFVF()))
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CIndexedVertexs::Render SetFVF error!");
                return(false);
            }

            if(S_OK != GM->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertexCount, 0, m_IndexCount / 3))
            {
                CLogger::GetSingletonPtr()->AddNewLog(ELL_ERROR, "CIndexedVertexs::Render DrawIndexedPrimitive error!");
                return(false);
            }

			return(true);
		}

		virtual inline unsigned short GetVertexType() const {return T::GetVertexType();}

};
#endif
