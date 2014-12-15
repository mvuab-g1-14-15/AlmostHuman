#include "AnimatedModels/AnimatedInstanceModel.h"

CAnimatedInstanceModel::CAnimatedInstanceModel()
{

}

CAnimatedInstanceModel::~CAnimatedInstanceModel()
{

}

void
CAnimatedInstanceModel::Render(CGraphicsManager *RM)
{

}

void
CAnimatedInstanceModel::RenderModelBySoftware(CGraphicsManager *RM)
{

}

void
CAnimatedInstanceModel::Update(float ElapsedTime)
{

}

void
CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *AnimatedCoreModel, CGraphicsManager *RM)
{

}

void
CAnimatedInstanceModel::Destroy()
{

}

void
CAnimatedInstanceModel::ExecuteAction(int Id, float DelayIn, float DelayOut, float WeightTarget, bool AutoLock)
{

}

void
CAnimatedInstanceModel::BlendCycle(int Id, float Weight, float DelayIn)
{

}

void
CAnimatedInstanceModel::ClearCycle(int Id, float DelayOut)
{

}

bool
CAnimatedInstanceModel::IsCycleAnimationActive(int Id) const
{

}

bool
CAnimatedInstanceModel::IsActionAnimationActive(int Id) const
{

}

bool
CAnimatedInstanceModel::LoadVertexBuffer(CGraphicsManager *RM)
{

}

void
CAnimatedInstanceModel::LoadTextures()
{

}
