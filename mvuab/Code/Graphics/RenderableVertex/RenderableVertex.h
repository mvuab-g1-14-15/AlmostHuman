#ifndef _RENDERABLE_VERTEX_H
#define _RENDERABLE_VERTEX_H
#pragma once

#include "Utils\Defines.h"
#include "GraphicsManager.h"
#include "Effects\EffectTechnique.h"

class CRenderableVertexs
{
    protected:
        LPDIRECT3DVERTEXBUFFER9 m_VB;
        LPDIRECT3DINDEXBUFFER9 m_IB;

        size_t m_IndexCount;
        size_t m_VertexCount;

    public:
        CRenderableVertexs()
            : m_IndexCount( 0 )
            , m_VertexCount( 0 )
            , m_VB( 0 )
            , m_IB( 0 )
        {
        }

        virtual ~CRenderableVertexs()
        {
            CHECKED_RELEASE(m_IB);
            CHECKED_RELEASE(m_VB);
        }

        virtual inline size_t GetFacesCount() const
        {
            return m_IndexCount / 3;
        }

        virtual inline size_t GetVertexsCount() const
        {
            return m_VertexCount;
        }

        virtual inline unsigned short GetVertexType() const = 0;
        virtual bool isRenderOK() { return false; }

        virtual bool Render(CGraphicsManager *GM, CEffectTechnique *effectTechnique, int baseVertexIndexCount, int minVertexIndex, int verticesCount, int startIndex, int facesCount) = 0;
        virtual bool Render(CGraphicsManager *GM, CEffectTechnique *EffectTechnique) = 0;
        virtual bool Render(CGraphicsManager *GM) = 0;

        virtual inline size_t GetVertexSize() = 0;
        virtual inline size_t GetIndexSize() = 0;
};
#endif
