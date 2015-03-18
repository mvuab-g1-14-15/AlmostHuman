#include "SceneRenderComands\DeveloperInfoSceneRendererCommand.h"
#include "GraphicsManager.h"
#include "Fonts\FontManager.h"
#include "Timer\Timer.h"
#include "Core.h"
#include "Cameras\Camera.h"
#include "Cameras\CameraManager.h"
#include "Utils\GPUStatics.h"

CDeveloperInfoSceneRenderCommand::CDeveloperInfoSceneRenderCommand( CXMLTreeNode& atts ) : CSceneRendererCommand( atts )
{
}

void CDeveloperInfoSceneRenderCommand::Execute( CGraphicsManager& GM )
{
  CFontManager* FM = CFontManager::GetSingletonPtr();
  // Put show all the information of the FPS
  uint32 width, height;
  GM.GetWidthAndHeight( width, height );
  Math::Vect2i WindowPosition( 10, height - 120 );
  RECT l_Rect = { 5, height - 140, 200, 200 };
  WindowPosition.y += FM->DrawDefaultText( WindowPosition.x, WindowPosition.y, Math::colGREEN, "FPS %5.2f", FPS );
  CCamera* lCurrentCamera = CCameraManager::GetSingletonPtr()->GetCurrentCamera();

  if ( lCurrentCamera )
  {
    Math::Vect3f lPosition = lCurrentCamera->GetPos();
    WindowPosition.y += FM->DrawDefaultText( WindowPosition.x, WindowPosition.y, Math::colYELLOW,
                        "Position < %5.2f %5.2f %5.2f > ", lPosition.x, lPosition.y, lPosition.z );
    Math::Vect3f lTarget = lCurrentCamera->GetLookAt();
    WindowPosition.y += FM->DrawDefaultText( WindowPosition.x, WindowPosition.y, Math::colYELLOW,
                        "Target < %5.2f %5.2f %5.2f > ", lTarget.x, lTarget.y, lTarget.z );
  }

  CGPUStatics* GPU = CGPUStatics::GetSingletonPtr();
  WindowPosition.y += FM->DrawDefaultText( WindowPosition.x, WindowPosition.y, Math::colRED,
                      "Objects %d", GPU->GetDrawCount() );
  WindowPosition.y += FM->DrawDefaultText( WindowPosition.x, WindowPosition.y, Math::colRED,
                      "Vertices %d", GPU->GetVertexCount() );
  WindowPosition.y += FM->DrawDefaultText( WindowPosition.x, WindowPosition.y, Math::colRED,
                      "Faces %d", GPU->GetFacesCount() );
  GPU->SetToZero();
}