#ifndef _INDEX_VERTEXS_H
#define _INDEX_VERTEXS_H
#pragma once

#include <Windows.h>
#include <DxErr.h>
#pragma comment(lib, "dxerr.lib")

#include "Utils\GPUStatics.h"
#include "RenderableVertex.h"

#include "Effects\Effect.h"
#include "Effects\EffectTechnique.h"

template<class T, class S> class CIndexedVertexs : public CRenderableVertexs
{
    private:
        bool m_RenderOK;

    protected:
        inline size_t GetVertexSize()
        {
            return sizeof( T );
        }
        inline size_t GetIndexSize()
        {
            return sizeof( S );
        }

        template<class I> D3DFORMAT GetIndexFormat()        { return D3DFMT_INDEX32; }
        template<>        D3DFORMAT GetIndexFormat<int>()   { return D3DFMT_INDEX32; }
        template<>        D3DFORMAT GetIndexFormat<short>() { return D3DFMT_INDEX16; }

    public:
        CIndexedVertexs( CGraphicsManager* GM, void* VertexAddress, void* IndexAddres, size_t VertexCount, size_t IndexCount ) : m_RenderOK(false)
        {
            if ( IndexCount != 0 || VertexCount != 0 )
            {
                void* l_memSrcV = 0;
                void* l_memSrcI = 0;

                m_IndexCount = IndexCount;
                m_VertexCount = VertexCount;

                HRESULT HR1 = GM->GetDevice()->CreateIndexBuffer( IndexCount * GetIndexSize(), 0, GetIndexFormat<S>(), D3DPOOL_DEFAULT, &m_IB, 0 );
                if(HR1 != D3D_OK)
                {
                    char s[256] = { 0 }; sprintf_s(s, sizeof(s), "ERROR Creating Index Buffer: %s\n", DXGetErrorString(HR1));
                    OutputDebugStringA(s);
                }

                HRESULT HR2 = GM->GetDevice()->CreateVertexBuffer( VertexCount * GetVertexSize(), 0, T::GetFVF(), D3DPOOL_DEFAULT, &m_VB, 0 );
                if(HR2 != D3D_OK)
                {
                    char s[256] = { 0 }; sprintf_s(s, sizeof(s), "ERROR Creating Vertex Buffer: %s\n", DXGetErrorString(HR2));
                    OutputDebugStringA(s);
                }
                
                if(m_VB != NULL && m_IB != NULL)
                {
                    m_VB->Lock( 0, VertexCount * GetVertexSize(), &l_memSrcV, 0 );
                    memcpy( l_memSrcV, VertexAddress, VertexCount * GetVertexSize() );
                    m_VB->Unlock();

                    m_IB->Lock( 0, IndexCount * GetIndexSize(), &l_memSrcI, 0 );
                    memcpy( l_memSrcI, IndexAddres, IndexCount * GetIndexSize() );
                    m_IB->Unlock();

                    m_RenderOK = true;
                }
            }
        }

        ~CIndexedVertexs()
        {
        }

        virtual bool isRenderOK()
        {
            return m_RenderOK;
        }

        virtual bool Render( CGraphicsManager* GM )
        {
            if(!m_RenderOK) return false;

            HRESULT lOk = GM->GetDevice()->SetStreamSource( 0, m_VB, 0, GetVertexSize() );
            lOk = GM->GetDevice()->SetIndices( m_IB );
            lOk = GM->GetDevice()->SetFVF( T::GetFVF() );
            lOk = GM->GetDevice()->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_VertexCount, 0, m_IndexCount / 3 );
            return lOk == S_OK;
        }

        virtual bool Render( CGraphicsManager* GM, CEffectTechnique* EffectTechnique, int baseVertexIndexCount, int minVertexIndex, int verticesCount, int startIndex, int facesCount )
        {
            if(!m_RenderOK) return false;

            LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetEffect();
            LPDIRECT3DDEVICE9 l_Device = GM->GetDevice();
            UINT l_NumPasses = 0;

            l_Effect->SetTechnique( EffectTechnique->GetD3DTechnique() );
            if ( FAILED( l_Effect->Begin( &l_NumPasses, 0 ) ) )
            {
                return false;
            }

            l_Device->SetVertexDeclaration( T::GetVertexDeclaration() );
            l_Device->SetStreamSource( 0, m_VB, 0, sizeof( T ) );
            l_Device->SetIndices( m_IB );

            for ( UINT b = 0; b < l_NumPasses; ++b )
            {
                l_Effect->BeginPass( b );
                l_Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, baseVertexIndexCount, minVertexIndex, verticesCount, startIndex, facesCount );
                l_Effect->EndPass();
            }

			CGPUStatics* GPU = CGPUStatics::GetSingletonPtr();
			GPU->AddVertexCount(verticesCount);
			GPU->AddFacesCount(facesCount);
			GPU->AddDrawCount(1);

            l_Effect->End();
            return ( true );
        }

        bool Render( CGraphicsManager* GM, CEffectTechnique* EffectTechnique )
        {
            if(!m_RenderOK) return false;

            EffectTechnique->BeginRender();
            LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetEffect();
            LPDIRECT3DDEVICE9 l_Device = GM->GetDevice();
            UINT l_NumPasses = 0;

            l_Effect->SetTechnique( EffectTechnique->GetD3DTechnique() );
            if ( FAILED( l_Effect->Begin( &l_NumPasses, 0 ) ) )
            {
                return false;
            }

            l_Device->SetVertexDeclaration( T::GetVertexDeclaration() );
            l_Device->SetStreamSource( 0, m_VB, 0, sizeof( T ) );
            l_Device->SetIndices( m_IB );

            for ( UINT b = 0; b < l_NumPasses; ++b )
            {
                l_Effect->BeginPass( b );
                l_Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, ( UINT ) m_VertexCount, 0, ( UINT ) m_IndexCount / 3 );
                l_Effect->EndPass();
            }

            l_Effect->End();

            return true;
        }

        virtual inline unsigned short GetVertexType() const
        {
            return T::GetVertexType();
        }
};
#endif
