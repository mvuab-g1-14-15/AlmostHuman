#include "SceneRenderComands\DeveloperInfoSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Fonts\FontManager.h"
#include "Timer\Timer.h"

#include "Cameras\Camera.h"
#include "Cameras\CameraManager.h"
#include "EngineManagers.h"

CDeveloperInfoSceneRenderCommand::CDeveloperInfoSceneRenderCommand( CXMLTreeNode& atts )
    : CSceneRendererCommand( atts )
    , m_Quad2dColor( 0.2f, 0.2f, 0.2f, 0.7f )
    , m_Quad2dEdgeColor( Math::colWHITE )
{
}

void CDeveloperInfoSceneRenderCommand::Execute( CGraphicsManager& GM )
{
  BROFILER_CATEGORY( "CDeveloperInfoSceneRenderCommand::Execute", Profiler::Color::Orchid )
    CFontManager* FM = FontInstance;
    int32 FontId = FM->GetTTF_Id( "Consolas" );
    uint32 width, height;
    GM.GetWidthAndHeight( width, height );
    CCamera* lCurrentCamera = CameraMInstance->GetCurrentCamera();
    Math::Vect3f lPosition = lCurrentCamera->GetPosition();
    Math::Vect3f lTarget = lCurrentCamera->GetLookAt();

    int32 lFontHeight = FM->SizeY( "FPS", FontId );
    GM.DrawRectangle2D( Math::Vect2i( 5, 7 ), width - 10, lFontHeight + 5, m_Quad2dColor, 2, 2, m_Quad2dEdgeColor );
    FM->DrawTextA( 8, 10, Math::colWHITE, FontId,
        "Stats: FPS <%5.2f> Cam<%5.2f %5.2f %5.2f> Look<%5.2f %5.2f %5.2f>",
        FPS, lPosition.x, lPosition.y, lPosition.z, lTarget.x, lTarget.y, lTarget.z );
}