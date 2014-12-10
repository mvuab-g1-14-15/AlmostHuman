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

			GM->GetDevice()->CreateIndexBuffer(IndexCount * GetIndexSize(), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_IB, 0);
			GM->GetDevice()->CreateVertexBuffer(VertexCount * GetVertexSize(), 0, T::GetFVF(), D3DPOOL_DEFAULT, &m_VB, 0);
			
			m_VB->Lock(0, VertexCount * GetVertexSize(), &l_memSrcV, 0);
			memcpy(l_memSrcV, VertexAddress, VertexCount * GetVertexSize());
			m_VB->Unlock();

			m_IB->Lock(0, IndexCount * GetIndexSize(), &l_memSrcI, 0);
			memcpy(l_memSrcI, IndexAddres, IndexCount * GetIndexSize());
			m_IB->Unlock();
		}

		~CIndexedVertexs()
		{
		}

		virtual bool Render(CGraphicsManager *GM)
		{
			GM->GetDevice()->SetIndices(m_IB);
			GM->GetDevice()->SetFVF(T::GetFVF());
            GM->GetDevice()->SetStreamSource(0, m_VB, 0, GetVertexSize());
			GM->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertexCount, 0, m_IndexCount / 3);

			return(true);
		}

		virtual inline unsigned short GetVertexType() const {return T::GetVertexType();}

};
#endif
