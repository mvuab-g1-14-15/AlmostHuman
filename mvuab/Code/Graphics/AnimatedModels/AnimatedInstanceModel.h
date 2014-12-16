#include "RenderableObject/RenderableObject.h"
#include "cal3d/cal3d.h"
#include "Utils/Name.h"

class CAnimatedCoreModel;
class CTexture;

class CAnimatedInstanceModel : public CRenderableObject
{

private:
    CalModel *m_CalModel;
    CAnimatedCoreModel *m_AnimatedCoreModel;
    // D3D vertex & index buffer
    LPDIRECT3DVERTEXBUFFER9 m_pVB;
    int m_VBCursor;
    LPDIRECT3DINDEXBUFFER9  m_pIB;
    int m_IBCursor;
    uint32 m_NumVtxs;
    uint32 m_NumFaces;
    uint32 m_CurrentAnimationId;
    float32 m_BlendTime;
    float32 m_LodLevel;
    bool LoadVertexBuffer(CGraphicsManager *RM);
    void LoadTextures();
public:
    CAnimatedInstanceModel(const std::string &Name, const std::string &CoreName);
    ~CAnimatedInstanceModel();
    void Render();
    void RenderModelBySoftware();
    void Update(float32 deltaTime);
    void Initialize();
    void Destroy();
    void ExecuteAction(uint32 Id, float32 DelayIn, float32 DelayOut, float32 WeightTarget=1.0f, bool AutoLock=true);
    void BlendCycle(uint32 Id, float32 Weight, float32 DelayIn);
    void ClearCycle(uint32 Id, float32 DelayOut);
    bool IsCycleAnimationActive(uint32 Id) const;
    bool IsActionAnimationActive(uint32 Id) const;
};