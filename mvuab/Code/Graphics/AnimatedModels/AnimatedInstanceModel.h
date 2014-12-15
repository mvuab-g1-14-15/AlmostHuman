#include "RenderableObject/RenderableObject.h"
#include "cal3d/cal3d.h"

class CAnimatedCoreModel;
class CTexture;

class CAnimatedInstanceModel : public CRenderableObject
{
private:
	CalModel *m_CalModel;
	CAnimatedCoreModel *m_AnimatedCoreModel;
	std::vector<CTexture *> m_TextureList;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;
	int m_NumVtxs;
	int m_NumFaces;
	bool LoadVertexBuffer(CGraphicsManager *RM);
	void LoadTextures();
public:
	CAnimatedInstanceModel();
	~CAnimatedInstanceModel();
	void Render(CGraphicsManager *RM);
	void RenderModelBySoftware(CGraphicsManager *RM);
	void Update(float ElapsedTime);
	void Initialize(CAnimatedCoreModel *AnimatedCoreModel, CGraphicsManager *RM);
	void Destroy();
	void ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget=1.0f, bool
		AutoLock=true);
	void BlendCycle(int Id, float Weight, float DelayIn);
	void ClearCycle(int Id, float DelayOut);
	bool IsCycleAnimationActive(int Id) const;
	bool IsActionAnimationActive(int Id) const;
};