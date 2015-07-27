#pragma once
#ifndef INSTANCING_VERTEX_H
#define INSTANCING_VERTEX_H

#include "RenderableVertex.h"
#include "Effects/Effect.h"
#include "Effects/EffectTechnique.h"

template<class T, class U>
class CInstancingVertexs : public CRenderableVertexs
{
    private:
        LPDIRECT3DVERTEXBUFFER9 m_InstanceB;
        int m_InstancesNumber;
    protected:
        inline size_t GetVertexSize()
        {
            return sizeof(T);
        }
        inline size_t GetIndexSize()
        {
            return sizeof(unsigned short);
        }
        inline size_t GetInstanceSize()
        {
            return sizeof(U);
        }
    public:
        void AddInstancinguffer(CGraphicsManager *RM, void *InstanceAddress)
        {
            void *instance_data;
            m_InstanceB->Lock(0, GetInstanceSize() * m_InstancesNumber, &instance_data, 0);
            memcpy(instance_data, InstanceAddress, GetInstanceSize() * m_InstancesNumber);
            m_InstanceB->Unlock();
        }

        CInstancingVertexs
        (
            CGraphicsManager *GM,
            void *VertexAddress,
            void *IndexAddres,
            size_t VertexCount,
            size_t IndexCount,
            size_t InstanceNumber
        )
        {
            if ( IndexCount != 0 || VertexCount != 0 )
            {
                void* l_memSrcV = 0;
                void* l_memSrcI = 0;
                m_IndexCount = IndexCount;
                m_VertexCount = VertexCount;
                m_InstancesNumber = InstanceNumber;
                GM->GetDevice()->CreateIndexBuffer( IndexCount * GetIndexSize(), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_IB, 0 );
                GM->GetDevice()->CreateVertexBuffer( VertexCount * GetVertexSize(), 0, T::GetFVF(), D3DPOOL_DEFAULT, &m_VB, 0 );

                m_VB->Lock( 0, VertexCount * GetVertexSize(), &l_memSrcV, 0 );
                memcpy( l_memSrcV, VertexAddress, VertexCount * GetVertexSize() );
                m_VB->Unlock();

                m_IB->Lock( 0, IndexCount * GetIndexSize(), &l_memSrcI, 0 );
                memcpy( l_memSrcI, IndexAddres, IndexCount * GetIndexSize() );
                m_IB->Unlock();

                GM->GetDevice()->CreateVertexBuffer(GetInstanceSize() * InstanceNumber, 0, 0, D3DPOOL_MANAGED, &m_InstanceB, 0);
            }
        }
        ~CInstancingVertexs()
        {
            CHECKED_RELEASE(m_InstanceB);
        }
        bool Render(CGraphicsManager *GM)
        {
            HRESULT lOk = GM->GetDevice()->SetStreamSource( 0, m_VB, 0, GetVertexSize() );
            lOk = GM->GetDevice()->SetIndices( m_IB );
            lOk = GM->GetDevice()->SetFVF( T::GetFVF() );
            lOk = GM->GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_VertexCount, 0, m_IndexCount / 3 );
            return lOk == S_OK;
        }

        virtual bool Render(CGraphicsManager *GM, CEffectTechnique *EffectTechnique)
        {
            LPDIRECT3DDEVICE9 l_Device = GM->GetDevice();
            UINT l_NumPasses;
            LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetEffect();
            l_Effect->SetTechnique(EffectTechnique->GetD3DTechnique());
            EffectTechnique->BeginRender();
            if (SUCCEEDED(l_Effect->Begin(&l_NumPasses, 0)))
            {
                l_Device->SetVertexDeclaration(T::GetVertexDeclaration());
                // Setting the first stream source and frequency
                l_Device->SetStreamSource(0, m_VB, 0, GetVertexSize());
                l_Device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_InstancesNumber);
                // Setting the second stream source and frequency
                l_Device->SetStreamSource(1, m_InstanceB, 0, GetInstanceSize());
                l_Device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1ul);
                l_Device->SetIndices(m_IB);
                for (UINT b = 0; b < l_NumPasses; ++b)
                {
                    l_Effect->BeginPass(b);
                    l_Device->DrawIndexedPrimitive
                    (
                        D3DPT_TRIANGLELIST,
                        0,
                        0,
                        (UINT)m_VertexCount,
                        0,
                        (UINT) m_IndexCount / 3
                    );
                    l_Effect->EndPass();
                }
                l_Effect->End();
            }
            l_Device->SetStreamSourceFreq(0, 1);
            l_Device->SetStreamSourceFreq(1, 1);
            return true;
        }

        virtual bool Render(CGraphicsManager *GM, CEffectTechnique *effectTechnique, int baseVertexIndexCount,
                            int minVertexIndex, int verticesCount, int startIndex, int facesCount)
        {
            return true;
        }

        bool Render(CGraphicsManager *GM, CEffectTechnique *EffectTechnique, int baseVertex = 0, int MinVertex = 0,
                    int NumVertex = -1, int StartIndex = 0, int IndexCount = -1) const
        {
            LPDIRECT3DDEVICE9 l_Device = GM->GetDevice();
            UINT l_NumPasses;
            LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetD3DEffect();
            l_Effect->SetTechnique(EffectTechnique->GetD3DTechnique());
            EffectTechnique->BeginRender();
            if (SUCCEEDED(l_Effect->Begin(&l_NumPasses, 0)))
            {
                l_Device->SetVertexDeclaration(T::GetVertexDeclaration());
                // Setting the first stream source and frequency
                l_Device->SetStreamSource(0, m_VB, 0, GetVertexSize());
                l_Device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_InstancesNumber);
                // Setting the second stream source and frequency
                l_Device->SetStreamSource(1, m_InstanceB, 0, GetInstanceSize());
                l_Device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1ul);
                l_Device->SetIndices(m_IB);
                for (UINT b = 0; b < l_NumPasses; ++b)
                {
                    l_Effect->BeginPass(b);
                    l_Device->DrawIndexedPrimitive
                    (
                        D3DPT_TRIANGLELIST,
                        baseVertex,
                        MinVertex,
                        NumVertex == -1 ? (UINT)m_VertexCount : (UINT)NumVertex,
                        StartIndex,
                        IndexCount == -1 ? (UINT) m_IndexCount / 3 : (UINT)IndexCount / 3
                    );
                    l_Effect->EndPass();
                }
                l_Effect->End();
            }
            l_Device->SetStreamSourceFreq(0, 1);
            l_Device->SetStreamSourceFreq(1, 1);
            return true;
        }
        virtual inline unsigned short GetVertexType() const
        {
            return T::GetVertexType();
        }
};

#endif