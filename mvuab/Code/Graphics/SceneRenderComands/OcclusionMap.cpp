#include "SceneRenderComands\OcclusionMap.h"

#include "Effects\EffectTechnique.h"
#include "Effects\EffectManager.h"
#include "Effects\Effect.h"

#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "RenderableObject\RenderableObjectTechnique.h"
#include "RenderableVertex\VertexTypes.h"

#include "RenderableObject\Scene.h"
#include "Cameras/CameraManager.h"
#include "Cameras/Camera.h"

#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Utils/Defines.h"

COcclusionMapSceneRendererCommand::COcclusionMapSceneRendererCommand(CXMLTreeNode& atts) : CStagedTexturedRendererCommand(atts)
{
}

COcclusionMapSceneRendererCommand::~COcclusionMapSceneRendererCommand()
{
}

void COcclusionMapSceneRendererCommand::Execute( CGraphicsManager & GM )
{
    ROTMInstance->GetPoolRenderableObjectTechniques().GetResource("occlusion_map_pool_renderable_object_technique")->Apply();
    CEngineManagers::GetSingletonPtr()->GetSceneManager()->RenderLayer("solid");
}
