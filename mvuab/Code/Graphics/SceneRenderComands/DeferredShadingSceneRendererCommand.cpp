#include "SceneRenderComands\DeferredShadingSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Core.h"
#include "RenderableObject\RenderableObjectTechniqueManager.h"
#include "Effects\Effect.h"

/*
<set_render_target name="deferred_multiple_render_target">
	<dynamic_texture stage_id="0" name="DiffuseMapTexture" texture_width_as_frame_buffer="true" format_type="A8R8G8B8"/>
	<dynamic_texture stage_id="1" name="LightMapTexture" texture_width_as_frame_buffer="true" format_type="A8R8G8B8"/>
	<dynamic_texture stage_id="2" name="NormalMapTexture" texture_width_as_frame_buffer="true" format_type="A8R8G8B8"/>
	<dynamic_texture stage_id="3" name="DepthMapTexture" texture_width_as_frame_buffer="true" format_type="R32F"/>
</set_render_target>
*/

CDeferredShadingSceneRendererCommand::CDeferredShadingSceneRendererCommand(CXMLTreeNode &atts)
    :CStagedTexturedRendererCommand(atts)
{
	m_RenderableObjectTechnique = CCore::GetSingletonPtr()->GetRenderableObjectTechniqueManager()->GetResource("Deferred");
}

CDeferredShadingSceneRendererCommand::~CDeferredShadingSceneRendererCommand()
{
}

void CDeferredShadingSceneRendererCommand::SetLightsData(CGraphicsManager &GM)
{
	CEffectTechnique* l_EffectTechnique = m_RenderableObjectTechnique->GetEffectTechnique();
	//l_EffectTechnique->SetupLights();
}

void CDeferredShadingSceneRendererCommand::Execute(CGraphicsManager &GM)
{
	CEffectTechnique* l_EffectTechnique = m_RenderableObjectTechnique->GetEffectTechnique();
	l_EffectTechnique->BeginRender();
	CEffect* l_Effect = l_EffectTechnique->GetEffect();
}