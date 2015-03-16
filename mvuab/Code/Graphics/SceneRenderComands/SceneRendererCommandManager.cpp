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
#include "SceneRenderComands\CaptureFrameBufferSceneRendererCommand.h"
#include "SceneRenderComands\EndRenderSceneRendererCommand.h"
#include "SceneRenderComands\DeferredShadingSceneRendererCommand.h"
#include "SceneRenderComands\RenderDebugSceneSceneRendererCommand.h"
#include "SceneRenderComands\RenderDebugLightsSceneRendererCommand.h"
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
#include "XML\XMLTreeNode.h"

#include "Core.h"
#include "GraphicsManager.h"

#include <Windows.h>
#include <sstream>
#include "Utils\ObjectFactory.h"


CSceneRendererCommandManager::CSceneRendererCommandManager()
{
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

bool CSceneRendererCommandManager::Load( const std::string& FileName )
{
  ObjectFactory1<CSceneRendererCommand, CXMLTreeNode, std::string > CommandFactory;

  // Register all the commands with the object factory class
  CommandFactory.Register( "begin_scene",
                           Type2Type<CBeginRenderSceneRendererCommand>( ) );
  CommandFactory.Register( "generate_shadow_maps",
                           Type2Type<CGenerateShadowMapsSceneRendererCommand>( ) );
  CommandFactory.Register( "clear_scene",
                           Type2Type<CClearSceneRendererCommand>( ) );
  CommandFactory.Register( "enable_z_write",
                           Type2Type<CEnableZWriteSceneRendererCommand>( ) );
  CommandFactory.Register( "enable_z_test",
                           Type2Type<CEnableZTestSceneRendererCommand>( ) );
  CommandFactory.Register( "set_matrices",
                           Type2Type<CSetMatricesSceneRendererCommand>( ) );
  CommandFactory.Register( "set_pool_renderable_objects_technique",
                           Type2Type<CRenderableObjectTechniquesSceneRendererCommand>( ) );
  CommandFactory.Register( "set_render_target",
                           Type2Type<CSetRenderTargetSceneRendererCommand>( ) );
  CommandFactory.Register( "disable_z_write",
                           Type2Type<CDisableZWriteSceneRendererCommand>( ) );
  CommandFactory.Register( "disable_z_test",
                           Type2Type<CDisableZTestSceneRendererCommand>( ) );
  CommandFactory.Register( "render_draw_quad",
                           Type2Type<CDrawQuadRendererCommand>( ) );
  CommandFactory.Register( "capture_frame_buffer",
                           Type2Type<CCaptureFrameBufferSceneRendererCommand>( ) );
  CommandFactory.Register( "end_scene",
                           Type2Type<CEndRenderSceneRendererCommand>( ) );
  CommandFactory.Register( "render_deferred_shading",
                           Type2Type<CDeferredShadingSceneRendererCommand>( ) );
  CommandFactory.Register( "render_debug_scene",
                           Type2Type<CRenderDebugSceneSceneRendererCommand>( ) );
  CommandFactory.Register( "render_debug_shadow_maps",
                           Type2Type<CRenderDebugShadowMapsSceneRendererCommand>( ) );
  CommandFactory.Register( "render_gui",
                           Type2Type<CRenderGUISceneRendererCommand>( ) );
  CommandFactory.Register( "present",
                           Type2Type<CPresentSceneRendererCommand>( ) );
  CommandFactory.Register( "enable_alpha_blend",
                           Type2Type<CEnableAlphaBlendSceneRendererCommand>( ) );
  CommandFactory.Register( "disable_alpha_blend",
                           Type2Type<CDisableAlphaBlendSceneRendererCommand>( ) );
  CommandFactory.Register( "gaussian",
                           Type2Type<CGaussianSceneRendererCommand>( ) );
  CommandFactory.Register( "lens_of_flare",
                           Type2Type<CLensOfFlareRendererCommand>( ) );
  CommandFactory.Register( "enable_alpha_test",
                           Type2Type<CEnableAlphaTestSceneRendererCommand>( ) );
  CommandFactory.Register( "set_blend",
                           Type2Type<CSetBlendSceneRendererCommand>( ) );
  CommandFactory.Register( "disable_alpha_test",
                           Type2Type<CDisableAlphaTestSceneRendererCommand>( ) );
  CommandFactory.Register( "render_scene",
                           Type2Type<CRenderSceneSceneRendererCommand>( ) );
  CommandFactory.Register( "render_debug_lights",
                           Type2Type<CRenderDebugLightsSceneRendererCommand>( ) );
  CommandFactory.Register( "render_developer_info",
                           Type2Type<CDeveloperInfoSceneRenderCommand>( ) );

  CXMLTreeNode l_File;

  if ( !l_File.LoadFile( FileName.c_str() ) )
  {
    std::string err = "ERROR reading the file " + FileName;
    MessageBox( NULL, err.c_str() , "Error", MB_ICONEXCLAMATION | MB_OK );
    exit( EXIT_FAILURE );
  }

  m_FileName = FileName;
  CXMLTreeNode  TreeNode = l_File["scene_renderer_commands"];

  if ( TreeNode.Exists() )
  {
    int count = TreeNode.GetNumChildren();

    for ( int i = 0; i < count; ++i )
    {
      std::string TagName = TreeNode( i ).GetName();

      if ( TagName == "comment" )
        continue;

      CSceneRendererCommand* Command = 0;

      if ( TagName == "unset_render_target" )
      {
        CSetRenderTargetSceneRendererCommand* SetRenderTargetSceneRendererCommand =
          dynamic_cast<CSetRenderTargetSceneRendererCommand*>( m_SceneRendererCommands.GetResource( TreeNode(
                i ).GetPszProperty( "render_target", "" ) ) );
        Command = new CUnsetRenderTargetSceneRendererCommand( SetRenderTargetSceneRendererCommand,
            TreeNode( i ) );
      }
      else
        Command = CommandFactory.Create( TagName.c_str(), TreeNode( i ) );

      if ( !Command )
      {
		  LOG_ERROR_APPLICATION("Comand %s not found in the factory of commands!", TagName.c_str());
      }
      else
      {
        if ( !m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name",
             GetNextName().c_str() ), Command ) )
          CHECKED_DELETE( Command );
      }
    }
  }

  return true;
}

bool CSceneRendererCommandManager::Execute()
{
  CGraphicsManager* gm = CCore::GetSingletonPtr()->GetGraphicsManager();
  std::vector<CSceneRendererCommand*>::iterator it =
    m_SceneRendererCommands.GetResourcesVector().begin(),
    it_end = m_SceneRendererCommands.GetResourcesVector().end();

  for ( ; it != it_end; ++it )
    ( *it )->Execute( *gm );

  return true;
}

bool CSceneRendererCommandManager::ReLoad()
{
  CleanUp();
  return Load( m_FileName );
}
