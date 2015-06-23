#ifndef _INDEX_VERTEXS_H
#define _INDEX_VERTEXS_H
#pragma once

#include <Windows.h>

#include "Utils\GPUStatics.h"
#include "RenderableVertex.h"

#include "Effects\Effect.h"
#include "Effects\EffectTechnique.h"

template<class T> class CIndexedVertexs : public CRenderableVertexs
{
    protected:
        inline size_t GetVertexSize()
        {
            return sizeof( T );
        }
        inline size_t GetIndexSize()
        {
            return sizeof( unsigned short );
        }

    public:
        CIndexedVertexs( CGraphicsManager* GM, void* VertexAddress, void* IndexAddres, size_t VertexCount, size_t IndexCount )
        {
            if ( IndexCount != 0 || VertexCount != 0 )
            {
                void* l_memSrcV = 0;
                void* l_memSrcI = 0;
                m_IndexCount = IndexCount;
                m_VertexCount = VertexCount;
                GM->GetDevice()->CreateIndexBuffer( IndexCount * GetIndexSize(), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_IB, 0 );
                GM->GetDevice()->CreateVertexBuffer( VertexCount * GetVertexSize(), 0, T::GetFVF(), D3DPOOL_DEFAULT, &m_VB, 0 );

                m_VB->Lock( 0, VertexCount * GetVertexSize(), &l_memSrcV, 0 );
                memcpy( l_memSrcV, VertexAddress, VertexCount * GetVertexSize() );
                m_VB->Unlock();

                m_IB->Lock( 0, IndexCount * GetIndexSize(), &l_memSrcI, 0 );
                memcpy( l_memSrcI, IndexAddres, IndexCount * GetIndexSize() );
                m_IB->Unlock();
            }
        }

        ~CIndexedVertexs()
        {
        }

        virtual bool Render( CGraphicsManager* GM )
        {
            LARGE_INTEGER l_timeFreq, l_lastTime, l_actualTime;
            QueryPerformanceCounter(&l_lastTime);
            QueryPerformanceFrequency(&l_timeFreq);

            CGPUStatics::GetSingletonPtr()->AddVertexCount( m_VertexCount );
            CGPUStatics::GetSingletonPtr()->AddFacesCount( m_IndexCount / 3 );
            CGPUStatics::GetSingletonPtr()->AddDrawCount( 1 );

            GM->GetDevice()->SetIndices( m_IB );
            GM->GetDevice()->SetFVF( T::GetFVF() );
            GM->GetDevice()->SetStreamSource( 0, m_VB, 0, GetVertexSize() );

            GM->GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_VertexCount, 0, m_IndexCount / 3 );

            QueryPerformanceCounter(&l_actualTime);
            QueryPerformanceFrequency(&l_timeFreq);
            double t = (double) (l_actualTime.QuadPart - l_lastTime.QuadPart) / (double) l_timeFreq.QuadPart;
            CGPUStatics::GetSingletonPtr()->AddTime((float) t * 1000.0f);

            return ( true );
        }

        virtual bool Render( CGraphicsManager* GM, CEffectTechnique* EffectTechnique, int baseVertexIndexCount,
                             int minVertexIndex, int verticesCount, int startIndex, int facesCount )
        {

            LARGE_INTEGER l_timeFreq, l_lastTime, l_actualTime;
            QueryPerformanceCounter(&l_lastTime);
            QueryPerformanceFrequency(&l_timeFreq);

            LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetEffect();
            LPDIRECT3DDEVICE9 l_Device = GM->GetDevice();
            UINT l_NumPasses = 0;

            l_Effect->SetTechnique( EffectTechnique->GetD3DTechnique() );
            if ( FAILED( l_Effect->Begin( &l_NumPasses, 0 ) ) )
            {
                QueryPerformanceCounter(&l_actualTime);
                QueryPerformanceFrequency(&l_timeFreq);
                double t = (double) (l_actualTime.QuadPart - l_lastTime.QuadPart) / (double) l_timeFreq.QuadPart;
                CGPUStatics::GetSingletonPtr()->AddTime((float) t * 1000.0f);

                return false;
            }

            CGPUStatics::GetSingletonPtr()->AddVertexCount( verticesCount );
            CGPUStatics::GetSingletonPtr()->AddFacesCount( facesCount );
            CGPUStatics::GetSingletonPtr()->AddDrawCount( l_NumPasses );

            l_Device->SetVertexDeclaration( T::GetVertexDeclaration() );
            l_Device->SetStreamSource( 0, m_VB, 0, sizeof( T ) );
            l_Device->SetIndices( m_IB );

            for ( UINT b = 0; b < l_NumPasses; ++b )
            {
                l_Effect->BeginPass( b );
                l_Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, baseVertexIndexCount, minVertexIndex, verticesCount, startIndex,
                                                facesCount );
                l_Effect->EndPass();
            }

            l_Effect->End();

            QueryPerformanceCounter(&l_actualTime);
            QueryPerformanceFrequency(&l_timeFreq);
            double t = (double) (l_actualTime.QuadPart - l_lastTime.QuadPart) / (double) l_timeFreq.QuadPart;
            CGPUStatics::GetSingletonPtr()->AddTime((float) t * 1000.0f);

            return ( true );
        }

        bool Render( CGraphicsManager* GM, CEffectTechnique* EffectTechnique )
        {
            LARGE_INTEGER l_timeFreq, l_lastTime, l_actualTime;
            QueryPerformanceCounter(&l_lastTime);
            QueryPerformanceFrequency(&l_timeFreq);

            EffectTechnique->BeginRender();
            LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetEffect();
            LPDIRECT3DDEVICE9 l_Device = GM->GetDevice();
            UINT l_NumPasses = 0;

            l_Effect->SetTechnique( EffectTechnique->GetD3DTechnique() );
            if ( FAILED( l_Effect->Begin( &l_NumPasses, 0 ) ) )
            {
                QueryPerformanceCounter(&l_actualTime);
                QueryPerformanceFrequency(&l_timeFreq);
                double t = (double) (l_actualTime.QuadPart - l_lastTime.QuadPart) / (double) l_timeFreq.QuadPart;
                CGPUStatics::GetSingletonPtr()->AddTime((float) t * 1000.0f);

                return false;
            }

            CGPUStatics::GetSingletonPtr()->AddVertexCount( m_VertexCount );
            CGPUStatics::GetSingletonPtr()->AddFacesCount( m_IndexCount / 3 );
            CGPUStatics::GetSingletonPtr()->AddDrawCount( l_NumPasses );

            l_Device->SetVertexDeclaration( T::GetVertexDeclaration() );
            l_Device->SetStreamSource( 0, m_VB, 0, sizeof( T ) );
            l_Device->SetIndices( m_IB );

            for ( UINT b = 0; b < l_NumPasses; ++b )
            {
                l_Effect->BeginPass( b );
                l_Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, ( UINT )m_VertexCount, 0, ( UINT ) m_IndexCount / 3 );
                l_Effect->EndPass();
            }

            l_Effect->End();

            QueryPerformanceCounter(&l_actualTime);
            QueryPerformanceFrequency(&l_timeFreq);
            double t = (double) (l_actualTime.QuadPart - l_lastTime.QuadPart) / (double) l_timeFreq.QuadPart;
            CGPUStatics::GetSingletonPtr()->AddTime((float) t * 1000.0f);

            return true;
        }

        virtual inline unsigned short GetVertexType() const
        {
            return T::GetVertexType();
        }
};
#endif
