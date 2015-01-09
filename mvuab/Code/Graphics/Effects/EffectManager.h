#pragma once
#ifndef INC_EFFECT_MANAGER_H
#define INC_EFFECT_MANAGER_H

#include "Math\Matrix44.h"
#include "Effects\Defines.h"
#include "Utils\MapManager.h"
#include "Utils\SingletonPattern.h"
#include <string.h>

class CEffectTechnique;
class CEffect;

class CEffectManager : public CMapManager<CEffectTechnique>, public CSingleton<CEffectManager>
{
public:
    CEffectManager();
    ~CEffectManager();
    const Math::Mat44f & GetWorldMatrix() const;
    const Math::Mat44f & GetProjectionMatrix() const;
    const Math::Mat44f & GetViewMatrix() const;
    const Math::Mat44f & GetViewProjectionMatrix();
    const Math::Vect3f & GetCameraEye();
    const Math::Mat44f & GetLightViewMatrix() const;
    const Math::Mat44f & GetShadowProjectionMatrix();
    void ActivateCamera( const Math::Mat44f &ViewMatrix, const Math::Mat44f &ProjectionMatrix, const Math::Vect3f &CameraEye);
    void SetWorldMatrix(const Math::Mat44f &Matrix);
    void SetProjectionMatrix(const Math::Mat44f &Matrix);
    void SetViewMatrix(const Math::Mat44f &Matrix);
    void SetViewProjectionMatrix(const Math::Mat44f &ViewProjectionMatrix);
    void SetLightViewMatrix(const Math::Mat44f &Matrix);
    void SetShadowProjectionMatrix(const Math::Mat44f &Matrix);
    void SetCameraEye(const Math::Vect3f &CameraEye);
    void Load(const std::string &FileName);
    void Reload();
    std::string GetTechniqueEffectNameByVertexDefault(unsigned short VertexType);
    size_t GetMaxLights() const;
    CEffect * GetEffect(const std::string &Name);
    CEffectTechnique * GetEffectTechnique(const std::string &Name);
    CEffectTechnique * GetStaticMeshTechnique() const;
    void SetStaticMeshTechnique(CEffectTechnique *StaticMeshTechnique);
    CEffectTechnique * GetAnimatedModelTechnique() const;
    void SetAnimatedModelTechnique(CEffectTechnique *AnimatedModelTechnique);
    void CleanUp();

private: // Members
    typedef std::map<int,std::string> TDefaultTechniqueEffectMap;
    TDefaultTechniqueEffectMap m_DefaultTechniqueEffectMap;
    
    Math::Mat44f m_WorldMatrix;
    Math::Mat44f m_ProjectionMatrix;
    Math::Mat44f m_ViewMatrix;
    Math::Mat44f m_ViewProjectionMatrix;
    Math::Mat44f m_LightViewMatrix;
    Math::Mat44f m_ShadowProjectionMatrix;
    Math::Vect3f m_CameraEye;
    
    CMapManager<CEffect> m_Effects;
    
    CEffectTechnique *m_StaticMeshTechnique;
    CEffectTechnique *m_AnimatedModelTechnique;

    std::string     m_Filename;

private: // Methods
};

#endif // INC_EFFECT_MANAGER_H