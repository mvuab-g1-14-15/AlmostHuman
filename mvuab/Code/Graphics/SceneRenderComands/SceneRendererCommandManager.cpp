#include "SceneRenderComands\SceneRendererCommandManager.h"
#include "SceneRenderComands\BeginRenderSceneRendererCommand.h"
#include "SceneRenderComands\ClearSceneRendererCommand.h"
#include "SceneRenderComands\EnableZWriteSceneRendererCommand.h"
#include "SceneRenderComands\EnableZTestSceneRendererCommand.h"
#include "SceneRenderComands\RenderableObjectTechniquesSceneRendererCommand.h"
#include "SceneRenderComands\StagedTexturedRendererCommand.h"
#include "SceneRenderComands\SetRenderTargetSceneRendererCommand.h"
#include "SceneRenderComands\RenderSceneSceneRendererCommand.h"
#include "SceneRenderComands\UnsetRenderTargetSceneRendererCommand.h"
#include "SceneRenderComands\DisableZWriteSceneRendererCommand.h"
#include "SceneRenderComands\DrawQuadRendererCommand.h"
#include "SceneRenderComands\DrawQuadByTechniqueRendererCommand.h"
#include "SceneRenderComands\CaptureFrameBufferSceneRendererCommand.h"
#include "SceneRenderComands\EndRenderSceneRendererCommand.h"
#include "SceneRenderComands\DeferredShadingSceneRendererCommand.h"
#include "SceneRenderComands\RenderDebugSceneSceneRendererCommand.h"
#include "SceneRenderComands\RenderDebugShadowMapsSceneRendererCommand.h"
#include "SceneRenderComands\RenderGUISceneRendererCommand.h"
#include "SceneRenderComands\PresentSceneRendererCommand.h"
#include "SceneRenderComands\SetMatricesSceneRendererCommand.h"
#include "SceneRenderComands\EnableAlphaBlendSceneRendererCommand.h"
#include "SceneRenderComands\DisableAlphaBlendSceneRendererCommand.h"
#include "SceneRenderComands\GaussianSceneRendererCommand.h"
#include "SceneRenderComands\LensOfFlareRendererCommand.h"
#include "SceneRenderComands\DisableAlphaTestSceneRendererCommand.h"
#include "SceneRenderComands\EnableAlphaTestSceneRendererCommand.h"
#include "SceneRenderComands\SetBlendSceneRendererCommand.h"
#include "SceneRenderComands\GenerateShadowMapsSceneRendererCommand.h"
#include "SceneRenderComands\DeveloperInfoSceneRendererCommand.h"
#include "SceneRenderComands\DisableZTestSceneRendererCommand.h"
#include "SceneRenderComands\ParticleRendererCommand.h"
#include "SceneRenderComands\WeaponTargetRendererCommand.h"
#include "SceneRenderComands\RenderDebugCommand.h"
#include "SceneRenderComands\RenderGizmosCommand.h"
#include "SceneRenderComands\BillboardRendererCommand.h"
#include "SceneRenderComands\ProcessRenderRendererCommand.h"
#include "SceneRenderComands\BloomSceneRendererCommand.h"
#include "SceneRenderComands\TriggerRendererCommand.h"
#include "XML\XMLTreeNode.h"
#include "EngineConfig.h"

#include "GraphicsManager.h"
#include "EngineManagers.h"
#include "Utils\Manager.h"


#include <Windows.h>
#include <sstream>


CSceneRendererCommandManager::CSceneRendererCommandManager()
    : CManager()
{
}

CSceneRendererCommandManager::CSceneRendererCommandManager( CXMLTreeNode& atts )
    : CManager( atts )
{
    /*  TODO RAUL
        LEER XML
    */
}

CSceneRendererCommandManager::~CSceneRendererCommandManager()
{
    m_SceneRendererCommands.Destroy();
}

void CSceneRendererCommandManager::CleanUp()
{
    m_SceneRendererCommands.Destroy();
}
std::string CSceneRendererCommandManager::GetNextName()
{
    std::stringstream l_Str;
    l_Str << "default_scene_renderer_command_";
    l_Str << m_SceneRendererCommands.GetResourcesVector().size();
    return l_Str.str();
}

void CSceneRendererCommandManager::Init()
{
    // Register all the commands with the object factory class
    m_CommandFactory.Register( "begin_scene",
                               Type2Type<CBeginRenderSceneRendererCommand>( ) );
    m_CommandFactory.Register( "generate_shadow_maps",
                               Type2Type<CGenerateShadowMapsSceneRendererCommand>( ) );
    m_CommandFactory.Register( "clear_scene",
                               Type2Type<CClearSceneRendererCommand>( ) );
    m_CommandFactory.Register( "enable_z_write",
                               Type2Type<CEnableZWriteSceneRendererCommand>( ) );
    m_CommandFactory.Register( "enable_z_test",
                               Type2Type<CEnableZTestSceneRendererCommand>( ) );
    m_CommandFactory.Register( "set_matrices",
                               Type2Type<CSetMatricesSceneRendererCommand>( ) );
    m_CommandFactory.Register( "set_pool_renderable_objects_technique",
                               Type2Type<CRenderableObjectTechniquesSceneRendererCommand>( ) );
    m_CommandFactory.Register( "set_render_target",
                               Type2Type<CSetRenderTargetSceneRendererCommand>( ) );
    m_CommandFactory.Register( "disable_z_write",
                               Type2Type<CDisableZWriteSceneRendererCommand>( ) );
    m_CommandFactory.Register( "disable_z_test",
                               Type2Type<CDisableZTestSceneRendererCommand>( ) );
    m_CommandFactory.Register( "render_draw_quad",
                               Type2Type<CDrawQuadRendererCommand>( ) );
    m_CommandFactory.Register( "render_draw_quad_by_technique",
                               Type2Type<CDrawQuadByTechniqueRendererCommand>( ) );
    m_CommandFactory.Register( "capture_frame_buffer",
                               Type2Type<CCaptureFrameBufferSceneRendererCommand>( ) );
    m_CommandFactory.Register( "end_scene",
                               Type2Type<CEndRenderSceneRendererCommand>( ) );
    m_CommandFactory.Register( "render_deferred_shading",
                               Type2Type<CDeferredShadingSceneRendererCommand>( ) );
    m_CommandFactory.Register( "render_debug_scene",
                               Type2Type<CRenderDebugSceneSceneRendererCommand>( ) );
    m_CommandFactory.Register( "render_debug_shadow_maps",
                               Type2Type<CRenderDebugShadowMapsSceneRendererCommand>( ) );
    m_CommandFactory.Register( "render_gui",
                               Type2Type<CRenderGUISceneRendererCommand>( ) );
    m_CommandFactory.Register( "present",
                               Type2Type<CPresentSceneRendererCommand>( ) );
    m_CommandFactory.Register( "enable_alpha_blend",
                               Type2Type<CEnableAlphaBlendSceneRendererCommand>( ) );
    m_CommandFactory.Register( "disable_alpha_blend",
                               Type2Type<CDisableAlphaBlendSceneRendererCommand>( ) );
    m_CommandFactory.Register( "gaussian",
                               Type2Type<CGaussianSceneRendererCommand>( ) );
    m_CommandFactory.Register( "lens_of_flare",
                               Type2Type<CLensOfFlareRendererCommand>( ) );
    m_CommandFactory.Register( "enable_alpha_test",
                               Type2Type<CEnableAlphaTestSceneRendererCommand>( ) );
    m_CommandFactory.Register( "set_blend",
                               Type2Type<CSetBlendSceneRendererCommand>( ) );
    m_CommandFactory.Register( "disable_alpha_test",
                               Type2Type<CDisableAlphaTestSceneRendererCommand>( ) );
    m_CommandFactory.Register( "render_scene",
                               Type2Type<CRenderSceneSceneRendererCommand>( ) );
    #ifdef _DEBUG
    m_CommandFactory.Register( "render_developer_info",
                               Type2Type<CDeveloperInfoSceneRenderCommand>( ) );
    #endif
    m_CommandFactory.Register( "render_particules",
                               Type2Type<CParticleRenderCommand>( ) );
    m_CommandFactory.Register( "render_weapon_target",
                               Type2Type<CWeaponTargetRendererCommand>( ) );
    m_CommandFactory.Register( "render_debug",
                               Type2Type<CRenderDebugCommand>( ) );
    m_CommandFactory.Register( "render_gizmos",
                               Type2Type<CRenderGizmosCommand>( ) );
    m_CommandFactory.Register( "render_gui",
                               Type2Type<CRenderGUISceneRendererCommand>( ) );
    m_CommandFactory.Register( "render_billboards",
                               Type2Type<CBillboardRenderCommand>( ) );
    m_CommandFactory.Register( "render_process",
                               Type2Type<CProcessRenderRendererCommand>( ) );
    m_CommandFactory.Register( "render_triggers",
                               Type2Type<CTriggerRenderCommand>( ) );
    m_CommandFactory.Register( "bloom", Type2Type<CBloomSceneRendererCommand>( ) );
    Load(mConfigPath);

}

bool CSceneRendererCommandManager::Load(const std::string& lFile )
{

    CXMLTreeNode l_Node, SceneRendererXml;
    if ( !SceneRendererXml.LoadAndFindNode(lFile.c_str(), "scene_renderer_commands", l_Node ) )
    {
        FATAL_ERROR( "Error reading the file %s", lFile.c_str() );
        return false;
    }

    bool lOk = true;

    for ( int i = 0, lCount = l_Node.GetNumChildren(); i < lCount ; ++i )
    {
        CXMLTreeNode& l_CurrentNode = l_Node( i );
        const std::string& l_TagName = l_CurrentNode.GetName();

        if ( l_TagName == "postproc" )
        {
            if ( !Load( l_CurrentNode.GetPszProperty( "filename", "" ) ) )
            {
                LOG_ERROR_APPLICATION( "Load->Error al intentar cargar el command: %s", l_TagName.c_str() );
                lOk = false;
            }
        }
        else
        {
            CSceneRendererCommand* Command = 0;
            if ( l_TagName == "unset_render_target" )
            {
                CSetRenderTargetSceneRendererCommand* SetRenderTargetSceneRendererCommand =
                    dynamic_cast<CSetRenderTargetSceneRendererCommand*>( m_SceneRendererCommands.GetResource(
                                l_CurrentNode.GetPszProperty( "render_target", "" ) ) );
                Command = new CUnsetRenderTargetSceneRendererCommand( SetRenderTargetSceneRendererCommand, l_CurrentNode );
                LOG_INFO_APPLICATION("Command added %s",  l_TagName.c_str() );
            }
            else
            {
                Command = m_CommandFactory.Create( l_TagName.c_str(), l_CurrentNode );
                LOG_INFO_APPLICATION("Command added %s",  l_TagName.c_str() );
            }

            if ( !Command )
            {
                LOG_ERROR_APPLICATION( "Command %s not found in the factory of commands!", l_TagName.c_str() );
                lOk = false;
            }
            else
            {
                if ( !m_SceneRendererCommands.AddResource( l_CurrentNode.GetPszProperty( "name", GetNextName().c_str() ), Command ) )
                {
                    CHECKED_DELETE( Command );
                }
            }
        }
    }

    return lOk;
}

bool CSceneRendererCommandManager::Execute()
{
    CGraphicsManager* gm = GraphicsInstance;
    std::vector<CSceneRendererCommand*>::iterator it =
        m_SceneRendererCommands.GetResourcesVector().begin(),
        it_end = m_SceneRendererCommands.GetResourcesVector().end();

    for ( ; it != it_end; ++it )
    {
        ( *it )->Execute( *gm );
    }

    return true;
}

void CSceneRendererCommandManager::ReLoad()
{
    CleanUp();
    Init();
}

CRenderGUISceneRendererCommand* CSceneRendererCommandManager::GetCommandGUI( )
{
    return dynamic_cast<CRenderGUISceneRendererCommand*>( m_SceneRendererCommands.GetResource( "render_gui" ) );
}
