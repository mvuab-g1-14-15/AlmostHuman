#ifndef ANIMATED_INSTANCE_MODEL_H
#define ANIMATED_INSTANCE_MODEL_H
#pragma once
#include "RenderableObject/RenderableObject.h"
#include "cal3d/cal3d.h"
#include "Utils/Name.h"
#include "AnimatedCoreModel.h"


class CEffectTechnique;
class CTexture;
class CXMLTreeNode;
class CRenderableObjectTechnique;

static const unsigned int gLightProbeSize = ( 4 * 6 * 2  )+ 4; // 4 light probes * 6 directions * 2 components in uv + 4 factors

class CAnimatedInstanceModel : public CRenderableObject
{
    public:
        CAnimatedInstanceModel( const std::string& Name, const std::string& CoreName );
        CAnimatedInstanceModel( const CXMLTreeNode& atts );
        ~CAnimatedInstanceModel();

        void Initialize();
        void Destroy();
        void Render();

        void RenderModelBySoftware();
        void RenderModelByHardware();

        Math::Vect3f GetBonePosition( const std::string& aBoneName );

        void Update();
        void ExecuteAction(uint32 Id, float32 DelayIn, float32 DelayOut, float32 WeightTarget = 1.0f, bool AutoLock = true);
        void RemoveAction(uint32 Id);

        void BlendCycle(uint32 Id, float32 Weight, float32 DelayIn);
        void ClearCycle(uint32 Id, float32 DelayOut);

        bool IsCycleAnimationActive(uint32 Id) const;
        bool IsActionAnimationActive(uint32 Id) const;

        void ExecuteAction(const std::string &AnimationName, float32 DelayIn, float32 DelayOut, float32 WeightTarget = 1.0f,
                           bool AutoLock = true);
        void ExecuteActionLUA(const std::string &AnimationName, float32 DelayIn, float32 DelayOut);
        void RemoveAction(const std::string &AnimationName);

        void BlendCycle(const std::string &AnimationName, float32 Weight, float32 DelayIn);
        void ClearCycle(const std::string &AnimationName, float32 DelayOut);

        bool IsCycleAnimationActive(const std::string &AnimationName) const;
        bool IsActionAnimationActive(const std::string &AnimationName) const;

        void ChangeAnimation(const std::string &AnimationName, float32 DelayIn, float32 DelayOut);
        void ChangeAnimationAction(const std::string &AnimationName, float32 DelayIn, float32 DelayOut);

        void SetAnimationState(const std::string& _szAnimationState);
        GET_SET(float32, Velocity);

    private:
        CalModel*             m_CalModel;
        CAnimatedCoreModel*   m_AnimatedCoreModel;

        LPDIRECT3DVERTEXBUFFER9 m_pVB;
        int                     m_VBCursor;
        
        LPDIRECT3DINDEXBUFFER9  m_pIB;
        int m_IBCursor;

        uint32 m_NumVtxs;
        uint32 m_NumFaces;
        uint32 m_CurrentAnimationId;
        float32     m_BlendTime;
        float32     m_ChangeAnimation;
        float32     m_Velocity;
        float32     m_fAnimationParameter;

        std::string m_szAnimationState;
        std::vector<CTexture*> m_Textures;
        CRenderableObjectTechnique*  m_RenderableObjectTechnique;
        
        float m_LPMatrixInitial[gLightProbeSize];
        float m_LPMatrixTarget[gLightProbeSize];
        Math::Vect3f m_PreviousPosition;

        void LoadTextures();

        typedef CAnimatedCoreModel::SAnimation       SAnimation;
        typedef CAnimatedCoreModel::SAction          SAction;
        typedef CAnimatedCoreModel::SCycle           SCycle;
        typedef CAnimatedCoreModel::SAnimationState  SAnimationState;
        typedef CAnimatedCoreModel::SAnimationChange SAnimationChange;

        void CalculateNewLightProbeMatrix();

        float ExecuteChange(const SAnimationChange& _AnimationChange);//retorna el fade
        void  ExecuteAction(const SAction& _Action);
        void  BlendCycle(const SCycle& _Cycle, float _fBlendTime);
        void  ClearCycle(const SCycle& _Cycle, float _fBlendTime);
};

#endif
