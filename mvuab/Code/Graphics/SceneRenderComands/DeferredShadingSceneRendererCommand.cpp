#include "SceneRenderComands\DeferredShadingSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Math\Color.h"
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

void CDeferredShadingSceneRendererCommand::Execute(CGraphicsManager &GM)
{
	ActivateTextures();

	for (size_t i = 0; i < m_StageTextures.size(); i++)
	{
		m_StageTextures[i].m_Texture->Activate(m_StageTextures[i].m_StageId);
	}

	SetLightsData(GM);
	/*
	CEffectTechnique* l_EffectTechnique = m_RenderableObjectTechnique->GetEffectTechnique();
	l_EffectTechnique->BeginRender();
	CEffect* l_Effect = l_EffectTechnique->GetEffect();*/
}


void CDeferredShadingSceneRendererCommand::SetLightsData(CGraphicsManager &GM)
{
	
	CLightManager* l_LightManager = CCore::GetSingletonPtr()->GetLightManager();
	
	uint32 l_Width, l_Height;
	GM.GetWidthAndHeight(l_Width, l_Height);

	RECT l_Rect = { 0, 0, (long)l_Width, (long)l_Height};

	/*
	CEffectTechnique* l_EffectTechnique = m_RenderableObjectTechnique->GetEffectTechnique();
	//l_EffectTechnique->SetupLights();*/
}