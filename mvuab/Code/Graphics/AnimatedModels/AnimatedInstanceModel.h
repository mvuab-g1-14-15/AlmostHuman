#ifndef ANIMATED_INSTANCE_MODEL_H
#define ANIMATED_INSTANCE_MODEL_H
#pragma once
#include "RenderableObject/RenderableObject.h"
#include "cal3d/cal3d.h"
#include "Utils/Name.h"

class CAnimatedCoreModel;
class CEffectTechnique;
class CTexture;
class CXMLTreeNode;
class CRenderableObjectTechnique;

class CAnimatedInstanceModel : public CRenderableObject
{
    public:
        CAnimatedInstanceModel( const std::string& Name, const std::string& CoreName );
        CAnimatedInstanceModel( CXMLTreeNode& atts );
        ~CAnimatedInstanceModel();

        void Initialize();
        void Destroy();
        void Render();

        void RenderModelBySoftware();
        void RenderModelByHardware();

        void Update();
        void ExecuteAction(uint32 Id, float32 DelayIn, float32 DelayOut, float32 WeightTarget = 1.0f, bool AutoLock = true);
        void RemoveAction(uint32 Id);

        void BlendCycle(uint32 Id, float32 Weight, float32 DelayIn);
        void ClearCycle(uint32 Id, float32 DelayOut);

        bool IsCycleAnimationActive(uint32 Id) const;
        bool IsActionAnimationActive(uint32 Id) const;

		void ExecuteAction(const std::string &AnimationName, float32 DelayIn, float32 DelayOut, float32 WeightTarget = 1.0f, bool AutoLock = true);
        void RemoveAction(const std::string &AnimationName);

        void BlendCycle(const std::string &AnimationName, float32 Weight, float32 DelayIn);
        void ClearCycle(const std::string &AnimationName, float32 DelayOut);

        bool IsCycleAnimationActive(const std::string &AnimationName) const;
        bool IsActionAnimationActive(const std::string &AnimationName) const;

		void ChangeAnimation(const std::string &AnimationName, float32 DelayIn, float32 DelayOut);

    private:
        CalModel*             m_CalModel;
        CEffectTechnique*     m_pEffectTechnique;
        CAnimatedCoreModel*   m_AnimatedCoreModel;

        LPDIRECT3DVERTEXBUFFER9 m_pVB;
        int m_VBCursor;

        std::vector<CTexture*> m_Textures;
        LPDIRECT3DINDEXBUFFER9  m_pIB;
        int m_IBCursor;

        uint32 m_NumVtxs;
        uint32 m_NumFaces;
        uint32 m_CurrentAnimationId;

        float32 m_LodLevel;
        float32 m_BlendTime;
        float32 m_ChangeAnimation;
        CRenderableObjectTechnique*  m_RenderableObjectTechnique;

        void LoadTextures();
};

#endif
