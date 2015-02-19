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
#include "XML\XMLTreeNode.h"

#include "Core.h"
#include "GraphicsManager.h"

#include <Windows.h>

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
  return "";//No lo entiendo
}

bool CSceneRendererCommandManager::Load( const std::string& FileName )
{
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

      if ( TagName == "begin_scene" )
      {
        CBeginRenderSceneRendererCommand* BeginRender = new CBeginRenderSceneRendererCommand( TreeNode(
              i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             BeginRender );
      }
      else if ( TagName == "clear_scene" )
      {
        CClearSceneRendererCommand* ClearScene = new CClearSceneRendererCommand( TreeNode( i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             ClearScene );
      }
      else if ( TagName == "enable_z_write" )
      {
        CEnableZWriteSceneRendererCommand* EnableZWrite = new CEnableZWriteSceneRendererCommand( TreeNode(
              i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             EnableZWrite );
      }
      else if ( TagName == "enable_z_test" )
      {
        CEnableZTestSceneRendererCommand* EnableZTest = new CEnableZTestSceneRendererCommand( TreeNode(
              i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             EnableZTest );
      }
      else if ( TagName == "set_matrices" )
      {
        CSetMatricesSceneRendererCommand* SetupMatrices = new CSetMatricesSceneRendererCommand( TreeNode(
              i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             SetupMatrices );
      }
      else if ( TagName == "set_pool_renderable_objects_technique" )
      {
        CRenderableObjectTechniquesSceneRendererCommand* SetPoolRenderableObjectTechnique = new
        CRenderableObjectTechniquesSceneRendererCommand( TreeNode( i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             SetPoolRenderableObjectTechnique );
      }
      else if ( TagName == "set_render_target" )
      {
        CSetRenderTargetSceneRendererCommand* SetRenderTarget = new CSetRenderTargetSceneRendererCommand(
          TreeNode( i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             SetRenderTarget );
      }
      else if ( TagName == "render_scene" )
      {
        CRenderSceneSceneRendererCommand* RenderScene = new CRenderSceneSceneRendererCommand( TreeNode(
              i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             RenderScene );
      }
      else if ( TagName == "unset_render_target" )
      {
        CSetRenderTargetSceneRendererCommand* SetRenderTargetSceneRendererCommand =
          dynamic_cast<CSetRenderTargetSceneRendererCommand*>
          ( m_SceneRendererCommands.GetResource( TreeNode( i ).GetPszProperty( "render_target",
              TagName.c_str() ) ) );
        CUnsetRenderTargetSceneRendererCommand* UnsetRenderTarget = new
        CUnsetRenderTargetSceneRendererCommand(
          SetRenderTargetSceneRendererCommand, TreeNode( i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             UnsetRenderTarget );
      }
      else if ( TagName == "disable_z_write" )
      {
        CDisableZWriteSceneRendererCommand* DisableZWrite = new CDisableZWriteSceneRendererCommand(
          TreeNode( i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             DisableZWrite );
      }
      else if ( TagName == "render_draw_quad" )
      {
        CDrawQuadRendererCommand* DrawQuad = new CDrawQuadRendererCommand( TreeNode( i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             DrawQuad );
      }
      else if ( TagName == "capture_frame_buffer" )
      {
        CCaptureFrameBufferSceneRendererCommand* CaptureFrameBuffer = new
        CCaptureFrameBufferSceneRendererCommand( TreeNode(
              i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             CaptureFrameBuffer );
      }
      else if ( TagName == "end_scene" )
      {
        CEndRenderSceneRendererCommand* EndScene = new CEndRenderSceneRendererCommand( TreeNode( i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             EndScene );
      }
      else if ( TagName == "render_deferred_shading" )
      {
        CDeferredShadingSceneRendererCommand* RenderDeferredShading = new
        CDeferredShadingSceneRendererCommand( TreeNode(
                                                i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             RenderDeferredShading );
      }
      else if ( TagName == "render_debug_scene" )
      {
        //PENDIENTE ATRIBUTOS layer no se como tratarlo
        CRenderDebugSceneSceneRendererCommand* RenderDebugScene = new CRenderDebugSceneSceneRendererCommand(
          TreeNode(
            i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             RenderDebugScene );
      }
      else if ( TagName == "render_debug_lights" )
      {
        CRenderDebugLightsSceneRendererCommand* RenderDebugScene = new
        CRenderDebugLightsSceneRendererCommand( TreeNode(
            i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             RenderDebugScene );
      }
      else if ( TagName == "render_debug_shadow_maps" )
      {
        //PENDIENTE ATRIBUTOS screen_width y screen_height que hacer con ellos
        CRenderDebugShadowMapsSceneRendererCommand* RenderDebugShadowMaps = new
        CRenderDebugShadowMapsSceneRendererCommand(
          TreeNode( i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             RenderDebugShadowMaps );
      }
      else if ( TagName == "render_gui" )
      {
        CRenderGUISceneRendererCommand* RenderGUI = new CRenderGUISceneRendererCommand( TreeNode( i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             RenderGUI );
      }
      else if ( TagName == "present" )
      {
        CPresentSceneRendererCommand* Present = new CPresentSceneRendererCommand( TreeNode( i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             Present );
      }
      else if ( TagName == "enable_alpha_blend" )
      {
        CEnableAlphaBlendSceneRendererCommand* l_EnableAlphaBlend = new
        CEnableAlphaBlendSceneRendererCommand( TreeNode(
            i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             l_EnableAlphaBlend );
      }
      else if ( TagName == "disable_alpha_blend" )
      {
        CDisableAlphaBlendSceneRendererCommand* l_DisableAlphaBlend = new
        CDisableAlphaBlendSceneRendererCommand( TreeNode(
            i ) );
        m_SceneRendererCommands.AddResource( TreeNode( i ).GetPszProperty( "name", TagName.c_str() ),
                                             l_DisableAlphaBlend );
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