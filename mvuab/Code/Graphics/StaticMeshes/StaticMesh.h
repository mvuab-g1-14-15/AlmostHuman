#pragma once
#ifndef INC_STATIC_MESH_H_
#define INC_STATIC_MESH_H_

#include <vector>
#include <string>

#include "Math/AABB.h"
#include "Utils/Defines.h"

class CRenderableVertexs;
class CGraphicsManager;
class CTexture;
class CRenderableObjectTechnique;

class CStaticMesh
{
    protected:
        typedef std::vector<CTexture*> TTextureVector;
        std::vector<TTextureVector> m_Textures;
        
        std::vector<CRenderableObjectTechnique*> m_RenderableObjectTechniques;
        std::vector<CRenderableVertexs*> m_RVs;

        std::vector<size_t> m_VertexTypes;
        std::vector<Math::Vect3f> m_VB;
        std::vector<uint32> m_IB;
        
        std::string m_FileName;
        std::string m_RenderableObjectTechniqueName;
        
        Math::AABB3f m_AABB;

        Math::Vect3f m_AABBCenter;
        bool m_IsTransformed;

        static uint32 mAllVtxCount;

    public:
        CStaticMesh();
        ~CStaticMesh();
        
        GET_SET(std::vector<CRenderableVertexs *>, RVs);

        const std::vector<Math::Vect3f> &GetVertexBuffer() { return m_VB; }
        const std::vector<uint32> &GetIndexBuffer() { return m_IB; }
        
        bool Load( const std::string& FileName );
        bool ReLoad();
        
        void Render( CGraphicsManager* GM );
        void Destroy();
        
        Math::AABB3f GetAABB() { return m_AABB; }
        void SetAABB(const Math::Vect3f &AABBCenter){ m_AABBCenter = AABBCenter; }
        Math::Vect3f GetAABBCenter(){ return m_AABBCenter; }

        bool GetRenderableObjectTechnique();

        bool IsTransformed() { return m_IsTransformed; }
        void setIsTransformed(bool isTransfomed){ m_IsTransformed = isTransfomed; }
};

#endif //INC_STATIC_MESH_H_